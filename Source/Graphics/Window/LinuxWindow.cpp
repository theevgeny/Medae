#include "LinuxWindow.hpp"

#if PLATFORM_DEC == 0x01 // LINUX

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <cstdint>

#include <spdlog/spdlog.h>

#include "Graphics/Window/WindowFacade.hpp"

using namespace Medae::Graphics;

static void applySizeHints(Display* dpy, ::Window win, WindowPropertiesFlags flags)
{
	XSizeHints sz = {};
	sz.flags = PMinSize | PMaxSize;

	if (flags & WindowPropertiesFlags::IS_RESIZABLE) {
		sz.min_width = 1;
		sz.min_height = 1;
		sz.max_width = 0; // unrestricted
		sz.max_height = 0;
	} else {
		XWindowAttributes wa;
		XGetWindowAttributes(dpy, win, &wa);
		sz.min_width = sz.max_width = wa.width;
		sz.min_height = sz.max_height = wa.height;
	}

	XSetWMNormalHints(dpy, win, &sz);
}

typedef struct
{
	unsigned long flags;
	unsigned long functions;
	unsigned long decorations;
	long inputMode;
	unsigned long status;
} MotifWmHints;

static void applyXHints(Display* dpy, ::Window win, XWMHints* inout_hints, WindowPropertiesFlags flags)
{
	XWMHints hints = *inout_hints;

	hints.flags |= InputHint;
	hints.input = (flags & WindowPropertiesFlags::ACCEPTS_FOCUS) ? True : False;

	hints.flags |= StateHint;
	hints.initial_state = (flags & WindowPropertiesFlags::INITIAL_MINIMAZED) ? IconicState : NormalState;

	XSetWMHints(dpy, win, &hints);
	*inout_hints = hints;
}

static MotifWmHints getHints(WindowPropertiesFlags flags)
{
	MotifWmHints hints{};
	hints.flags = 0x00000001 /* MWM_HINTS_FUNCTIONS   */
		| 0x00000002 /* MWM_HINTS_DECORATIONS */;

	unsigned long func = 0;
	unsigned long decor = 0;

	if (flags & WindowPropertiesFlags::IS_BORDERLESS) {
		hints.decorations = 0;
		func = 0x00000002 // RESIZE
			| 0x00000004 // MOVE
			| 0x00000008 // MINIMIZE
			| 0x00000010 // MAXIMIZE
			| 0x00000020; // CLOSE
	} else {
		if (flags & WindowPropertiesFlags::HAS_BORDER)
			decor |= 0x00000002;
		if (flags & WindowPropertiesFlags::IS_RESIZABLE)
			decor |= 0x00000004;
		if (flags & WindowPropertiesFlags::HAS_HEADER)
			decor |= 0x00000008;
		if (flags & WindowPropertiesFlags::HAS_SYSTEM_MENU)
			decor |= 0x00000010;
		if (flags & WindowPropertiesFlags::HAS_MINIMIZE_BUTTON)
			decor |= 0x00000020;
		if (flags & WindowPropertiesFlags::HAS_MAXIMIZE_BUTTON)
			decor |= 0x00000040;

		if (flags & WindowPropertiesFlags::IS_RESIZABLE)
			func |= 0x00000002;
		if (flags & WindowPropertiesFlags::IS_MOVABLE)
			func |= 0x00000004;
		if (flags & WindowPropertiesFlags::HAS_MINIMIZE_BUTTON)
			func |= 0x00000008;
		if (flags & WindowPropertiesFlags::HAS_MAXIMIZE_BUTTON)
			func |= 0x00000010;
		if (flags & WindowPropertiesFlags::HAS_CLOSE_BUTTON)
			func |= 0x00000020;
	}

	hints.decorations = decor;
	hints.functions = func;
	return hints;
}

static void applyMotifHints(Display* dpy, ::Window win, WindowPropertiesFlags flags)
{
	MotifWmHints m = getHints(flags);
	Atom motifAtom = XInternAtom(dpy, "_MOTIF_WM_HINTS", False);
	XChangeProperty(dpy, win, motifAtom, motifAtom, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&m), 5);
}

static void applyNetWmState(Display* dpy, ::Window win, WindowPropertiesFlags flags)
{
	Atom wmState = XInternAtom(dpy, "_NET_WM_STATE", False);
	Atom atoms[4];
	int n = 0;

	if (flags & WindowPropertiesFlags::IS_TOPMOST)
		atoms[n++] = XInternAtom(dpy, "_NET_WM_STATE_ABOVE", False);
	if (flags & WindowPropertiesFlags::IS_BOTTOM)
		atoms[n++] = XInternAtom(dpy, "_NET_WM_STATE_BELOW", False);
	if (flags & WindowPropertiesFlags::IS_FULLSCREEN)
		atoms[n++] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);

	if (n > 0) {
		XChangeProperty(dpy, win, wmState, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(atoms), n);
	}
}

LinuxWindow::LinuxWindow(uint32_t width, uint32_t height, const char* name, WindowPropertiesFlags flags)
	: m_width(width), m_height(height), m_flags(flags)
{
	Display* dpy = XOpenDisplay(nullptr);
	if (!dpy) {
		spdlog::error("[LinuxWindow] Failed to open X display (DISPLAY env?)");
		return;
	}

	const int screen = DefaultScreen(dpy);
	const ::Window root = RootWindow(dpy, screen);

	const unsigned long black = BlackPixel(dpy, screen);
	const unsigned long white = WhitePixel(dpy, screen);

	::Window win = XCreateSimpleWindow(
		dpy, root, 0, 0, static_cast<unsigned int>(width), static_cast<unsigned int>(height), 0, black, white);

	if (!win) {
		spdlog::error("[LinuxWindow] XCreateSimpleWindow failed");
		XCloseDisplay(dpy);
		return;
	}

	if (name) {
		XStoreName(dpy, win, name);
		XSetIconName(dpy, win, name);
	}

	XSelectInput(dpy, win,
		StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
			PointerMotionMask | FocusChangeMask);

	Atom wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &wmDelete, 1);

	XWMHints wmHints{};
	applyXHints(dpy, win, &wmHints, flags);
	applyMotifHints(dpy, win, flags);
	applyNetWmState(dpy, win, flags);
	applySizeHints(dpy, win, flags);

	if (flags & WindowPropertiesFlags::IS_TRANSPARENT_FOR_INPUT) {
		int evBase = 0, errBase = 0;
		if (XShapeQueryExtension(dpy, &evBase, &errBase)) {
			XRectangle empty{0, 0, 0, 0};
			XShapeCombineRectangles(dpy, win, ShapeInput, 0, 0, &empty, 0, ShapeSet, Unsorted);
		} else {
			spdlog::warn("[LinuxWindow] XShape unavailable, IS_TRANSPARENT_FOR_INPUT ignored");
		}
	}

	XMapWindow(dpy, win);
	XFlush(dpy);

	m_rawPtrs[0] = reinterpret_cast<void*>(dpy);
	m_rawPtrs[1] = reinterpret_cast<void*>(static_cast<uintptr_t>(win));

	spdlog::info("[LinuxWindow] created {}x{} '{}' display={:p} win=0x{:x}", width, height, name ? name : "<null>",
		static_cast<void*>(dpy), static_cast<unsigned long>(win));
}

LinuxWindow::~LinuxWindow()
{
	if (!m_rawPtrs[0])
		return;

	auto* dpy = reinterpret_cast<Display*>(m_rawPtrs[0]);
	auto win = static_cast<::Window>(reinterpret_cast<uintptr_t>(m_rawPtrs[1]));

	if (dpy && win) {
		XUnmapWindow(dpy, win);
		XDestroyWindow(dpy, win);
	}
	if (dpy) {
		XCloseDisplay(dpy);
	}

	m_rawPtrs[0] = nullptr;
	m_rawPtrs[1] = nullptr;
}

void* LinuxWindow::getRaw()
{
	return m_rawPtrs;
}

void LinuxWindow::resize(uint32_t width, uint32_t height)
{
	m_width = width;
	m_height = height;

	if (!m_rawPtrs[0])
		return;

	auto* dpy = reinterpret_cast<Display*>(m_rawPtrs[0]);
	auto win = static_cast<::Window>(reinterpret_cast<uintptr_t>(m_rawPtrs[1]));
	if (!dpy || !win)
		return;

	XResizeWindow(dpy, win, static_cast<unsigned int>(width), static_cast<unsigned int>(height));
	applySizeHints(dpy, win, m_flags);
	XFlush(dpy);
}

WindowPropertiesFlags LinuxWindow::getFlags()
{
	return m_flags;
}
uint32_t LinuxWindow::getWidth()
{
	return m_width;
}
uint32_t LinuxWindow::getHeight()
{
	return m_height;
}

#endif