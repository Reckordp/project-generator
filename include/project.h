#define CURSOR_INSERT_MODE ("\x1b[5 q")
#define CURSOR_SAVE_POS ("\x1b[s")
#define CURSOR_RESTORE_POS ("\x1b[u")
#define CURSOR_THIN_BAR ("\x1b[6 q")
#define PANGKALAN_CODING ("D:\\Coding")
#define PANJANG_TAHAN_AWAL (256)

extern const char *templateRakefile;

typedef struct {
  const char *program;
  size_t jumlahargs;
  const char **argument;
  int pid;
} rincianproses;

void mulai_proses(rincianproses*);
void mulai_clone(const char*);