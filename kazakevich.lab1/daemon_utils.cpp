#include "daemon_utils.h"
#include <iostream>

void daemonize() {

    pid_t pid = fork();

    if (pid < 0) {
        //std::cerr << "Error fork pid < 0" << std::endl;
        exit(EXIT_FAILURE);  // ������ fork
    }

    if (pid > 0) {
        //std::cerr << "End parent process" << std::endl;
        exit(EXIT_SUCCESS);  // ��������� ������������ �������
    }

    //std::cerr << "Create new s" << std::endl;
    // ������� ����� �����
    if (setsid() < 0) {
        std::cerr << "setsid() < 0 error" << std::endl;
        exit(EXIT_FAILURE);
    }
    //std::cerr << "ignore signals" << std::endl;
    // ���������� �������
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

   // ������ fork ��� �������������� ����������� ����������� �� ���������
    pid = fork();
    if (pid < 0) {
        //std::cerr << "second fork error pid < 0" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        //std::cerr << "second fork error pid > 0" << std::endl;
        exit(EXIT_SUCCESS);
    }

    //std::cerr << "Set umask" << std::endl;
    // ������������� umask � ������� ����������
    umask(0);

    //std::cerr << "chdir" << std::endl;
    if (chdir("/") < 0) {
        //std::cerr << "chdir error" << std::endl;
        exit(EXIT_FAILURE);
    }

    //std::cerr << "close files" << std::endl;
    // ��������� ����������� �������� �����������
    //close(STDIN_FILENO);
    //close(STDOUT_FILENO);
    //close(STDERR_FILENO);

    //std::cerr << "open" << std::endl;
    // �������������� ����������� ����������� �� /dev/null
    //open("/dev/null", O_RDONLY); // STDIN
    //open("/dev/null", O_RDWR);   // STDOUT
    //open("/dev/null", O_RDWR);   // STDERR
    //std::cerr << "end demonize" << std::endl;
}