#define CHANNEL 8

#ifdef __cplusplus
extern "C" {
#endif

int InitLog(void);

extern int PID;

// режимы работы, задавемые в командной строке
#define runDEBUG  0 // отладка
#define runLOOP   1 // адаптивное управление
#define runACH    2 // АЧХ
#define runSTH    3 // статическая характеристика
#define runWZS    4 // взаимодействие в статике
#define runNMDL   5 // подготовка нагружения

#ifdef __cplusplus
}
#endif

