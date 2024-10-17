#include "daemon_utils.h"

void daemonize() {
    pid_t pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);  // ������ fork
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);  // ��������� ������������ �������
    }

    // ������� ����� �����
    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    // ���������� �������
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // ������ fork ��� �������������� ����������� ����������� �� ���������
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // ������������� umask � ������� ����������
    umask(0);
    if (chdir("/") < 0) {
        exit(EXIT_FAILURE);
    }

    // ��������� ����������� �������� �����������
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // �������������� ����������� ����������� �� /dev/null
    open("/dev/null", O_RDONLY); // STDIN
    open("/dev/null", O_RDWR);   // STDOUT
    open("/dev/null", O_RDWR);   // STDERR
}