#define MAX_VISITORS 350
#define MAN 1
#define WOMAN 0

//#define WINDOWS 1
#define MAC 1

#ifdef WINDOWS
#include <windows.h>
#include <stdio.h>
#include <sys\timeb.h> 
DWORD WINAPI visitor(void* data);
HANDLE  threads[MAX_CAPACITY_VISITOR] = { NULL };
HANDLE mutex;
#endif
#ifdef MAC
#include <mach/mach_time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
void* visitor(void* data);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#endif







struct person {
	int gender;
	int age;
};

int visitor_qty = 0;
int rutsche_1 = 0;
int kinderbecken = 0;
int schwimmbecken = 0;

#ifdef WINDOWS
int main()
{
	mutex = CreateMutex(NULL, FALSE, NULL);
	
	struct person id;

	for (int i = 0; i < MAX_CAPACITY_VISITOR; i++) {
		id.age = i % 20;
		id.gender = i % 2;
		if (!(threads[i] = CreateThread(NULL, 0, visitor, (LPVOID)&id, 0, NULL))) {
			printf("\nCould not create thread\n");
		}
	}
	Sleep(1000);
	printf("\nkinderbecken: %d\nschwimmbecken: %d\n", kinderbecken, schwimmbecken);
	//WaitForMultipleObjects(MAX_CAPACITY_VISITOR, threads, TRUE, INFINITE);

	for (int i = 0; i < 10; i++) {
		CloseHandle(threads[i]);
	}
	CloseHandle(mutex);
}
#endif
#ifdef MAC
int main() {
	struct person id;
	pthread_t thread[MAX_VISITORS];
	for (int i = 0; i < MAX_VISITORS; i++) {
		id.age = i % 20;
		id.gender = i % 2;
		pthread_create(&thread[i], 0, visitor, (void*)&id);
		pthread_join(thread[i], 0);
	}
	printf("%d\n", schwimmbecken);
	printf("%d\n", kinderbecken);

	return 0;

}
#endif

#ifdef WINDOWS
void lock_mutex()
{
	WaitForSingleObject(mutex, INFINITE);
	return;
}

void unlock_mutex()
{
	ReleaseMutex(mutex);
	return;
}
#endif
#ifdef MAC
void lock_mutex() {
	// Lock the mutexs
	pthread_mutex_lock(&mutex);
}

void unlock_mutex() {
	// Unlock the mutex
	pthread_mutex_unlock(&mutex);
}
#endif

void visitor_coming()
{
	lock_mutex();
	visitor_qty++;
	unlock_mutex();
}
void visitor_leaving()
{
	lock_mutex();
	visitor_qty--;
	unlock_mutex();
}

#ifdef WINDOWS
void wait(float sim_seconds)
{
	float real_seconds = sim_seconds / 60;
	Sleep(real_seconds * 1000);
}
#endif

#ifdef MAC
void wait(int sim_seconds) {
	float real_seconds = sim_seconds / 60;
	usleep(real_seconds * 1000000);
}
#endif

void go_into_kinderbecken()
{
	lock_mutex();
	kinderbecken++;
	unlock_mutex();
}

void go_into_schwimmbecken()
{
	lock_mutex();
	schwimmbecken++;
	unlock_mutex();
}

enum actions {
	KINDER_BECKEN, SCHWIMMBECKEN, RUTSCHE_1, RUTSCHE_2, NONE
};

enum actions make_decision(struct person id)
{
	enum actions ret = NONE;
	return ret;
}

#ifdef MAC
void* visitor(void* data) {
#endif
#ifdef WINDOWS
DWORD WINAPI visitor(void *data)
{
#endif
	visitor_coming();

	struct person id;
	id = *((struct person *)data);

	enum actions action;
	int active = 1;

	while (active) {
		action = make_decision(id);
		switch (action) {
		case KINDER_BECKEN:
			break;
		case SCHWIMMBECKEN:
			go_into_schwimmbecken();
			break;
		case RUTSCHE_1:
			break;
		case RUTSCHE_2:
			break;
		case NONE:
			break;
		default:
			break;
		}
	}
	visitor_leaving();
	return 0;
}

//struct timeb start, end;
	//ftime(&start);
	//ftime(&end);
	//printf("\ntime: %d", end.millitm - start.millitm);