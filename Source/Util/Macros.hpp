#define STRINGIZE(x) #x
#define STRINGIZE_LINE(x) STRINGIZE(x)
#define FILEPOINT "(" __FILE__ ":" STRINGIZE_LINE(__LINE__) ")"
#define NODIS [[nodiscard]]