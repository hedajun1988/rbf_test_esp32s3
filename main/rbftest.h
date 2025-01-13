
#ifndef RBF_TEST_H
#define RBF_TEST_H

typedef struct stCmd {
	const char *name;
	void (*func)(char *argv[], int argc);
	const char *desc;
}stCmd_t;


void func(int cmd);
void* cmd_thread(void* arg);
stCmd_t *cmd_search(const char *cmd);


void func(int cmd);

void do_cmd_help(char *argv[], int argc);
void printenv(char *argv[], int argc);
void test_enroll(char *argv[], int argc);
void test_stop_enroll(char *argv[], int argc);
void test_ledset(char *argv[], int argc);
void test_delete(char *argv[], int argc);
void test_list(char *argv[], int argc);
void test_sounder(char *argv[], int argc);
void test_pir(char *argv[], int argc);
void test_temphumi(char *argv[], int argc);
void delete(char *argv[], int argc);
void updev(char *argv[], int argc);
void exit_rbf(char *argv[], int argc);
void devarm(char *argv[], int argc);
void pir_option(char *argv[], int argc);
void findme(char *argv[], int argc);
void stopfindme(char *argv[], int argc);
void rssi(char *argv[], int argc);
void stoprssi(char *argv[], int argc);
void init_rbf(char *argv[], int argc);


int test_rbf_init(void);

#endif