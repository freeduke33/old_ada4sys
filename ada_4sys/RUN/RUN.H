#define CHANNEL 8

#ifdef __cplusplus
extern "C" {
#endif

int InitLog(void);

extern int PID;

// ०��� ࠡ���, �������� � ��������� ��ப�
#define runDEBUG  0 // �⫠���
#define runLOOP   1 // ����⨢��� �ࠢ�����
#define runACH    2 // ���
#define runSTH    3 // ����᪠� �ࠪ���⨪�
#define runWZS    4 // ����������⢨� � ��⨪�
#define runNMDL   5 // �����⮢�� ����㦥���

#ifdef __cplusplus
}
#endif

