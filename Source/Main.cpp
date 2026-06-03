#include <Medae.hpp>

int main(int argc, char **argv) {
    Medae::initialize(ProgramArgs(argc, argv));
    Medae::getInstance().getMainThread().join();
}