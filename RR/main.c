#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PROCESS   100
#define MAX_QUEUE  100



typedef int(*comp)(const void*, const void*);
//tail..........head
typedef struct process {
	int pid; //프로세스번호
	int ar_t; //도착시간
	int ser_t; //수행시간
	int remain_time; //수행후 남아있는 시간
	int waiting_time; //대기시간
	int end_time; //종료시간
	int end_point; //종료확인 변수
	int recoverycnt; // 회복
	double j_time; //정규화된 반환시간
} process;

process g_process[MAX_PROCESS];
process* prc_queue[MAX_QUEUE] = { NULL };
int q_head = 0;
int q_tail = -1;
int pr_cnt = -1; //프로세스 현재 번호

//RR 알고리즘
void RR(int cnt, process s[]) { //프로세스 개수 cnt
	//printf("%d \n",g_process[1].remain_time);

	int t = 0, i = 0, end_cnt;
	int flag = 1;
	int time_q = 1, time = 0, q = 1;

	int exit = 0; //종료 프로세스 개수
	int average_wait = 0;
	double average_banhwan = 0;

	for (i = 0; i < cnt; i++) {
		g_process[i].end_point = 1;
	}

	end_cnt = cnt;

	while (flag) {
		for (i = 0; i < cnt; i++) {
			//printf("\n_______service Time : %d_________\n\n", time + 1); // 삭제


			if (g_process[i].remain_time > 0 && g_process[i].ar_t <= time) {
				if (g_process[i].pid == 2 || g_process[i].pid == 4) { //2.4 번 Process 순서 교체 알고리즘 Start
					if (time > 5 && time < 19) {
						//4 start
						if (g_process[i].pid == 4) {
							if (g_process[i - 2].remain_time == 0 && (g_process[i - 2].end_point == 1)) {
								g_process[i - 2].end_time = time;
								g_process[i - 2].end_point = 0;
								end_cnt -= 1;
								printf("Process ID : 2 완료\n");

								g_process[i - 2].waiting_time = g_process[i - 2].end_time - (g_process[i - 2].ser_t + g_process[i - 2].ar_t);
								g_process[i - 2].j_time = (g_process[i - 2].waiting_time + g_process[i - 2].ser_t) / (double)g_process[i - 2].ser_t;

								printf("%d\t\t %ds\t\t %ds\t\t %ds\t\t %ds\t\t %.2fs \n\n", g_process[i - 2].pid, g_process[i - 2].ar_t, g_process[i - 2].ser_t,
									g_process[i - 2].end_time, g_process[i - 2].end_time - g_process[i - 2].ar_t, g_process[i - 2].j_time);
							}
							else {
								//printf("========ID : %d========\n", g_process[1].pid);
								g_process[1].remain_time -= 1;
								//printf("수행후 남아있는 시간  %d\n", g_process[1].remain_time);
								time++;
								if (g_process[1].remain_time < 0) { //회복
									g_process[i].recoverycnt += 1;
								}
							}
						}
						//4 end
						//2 start
						else if ((g_process[i].pid == 2)) {
							if (g_process[3].remain_time == 0 && (g_process[3].end_point == 1)) {
								g_process[3].end_time = time;
								g_process[3].end_point = 0;
								end_cnt -= 1;
								//printf("4번완료");
							}
							else {
								//printf("========ID : %d========\n", g_process[3].pid);
								g_process[3].remain_time -= 1;
								//printf("수행후 남아있는 시간 %d\n", g_process[3].remain_time);
								time++;
								if (g_process[3].remain_time < 0) { // 회복
									g_process[i].recoverycnt += 1;
								}
							}
						}
						//2end

					} //2.4 번 Process 순서 교체 알고리즘 End

					else {
						//printf("========ID : %d========\n", g_process[i].pid);
						//printf("수행전 남아있는 시간 %d\n", g_process[i].remain_time);
						g_process[i].remain_time -= 1;
						//printf("수행후 남아있는 시간 %d\n", g_process[i].remain_time);
						time++;
					}

				}
				else {
					//printf("========ID : %d========\n", g_process[i].pid);
					//printf("수행전 남아있는 시간 %d\n", g_process[i].remain_time);
					g_process[i].remain_time -= 1;
					//printf("수행후 남아있는 시간 %d\n", g_process[i].remain_time);
					time++;
				}



				/*check end Point*/
				if ((g_process[i].remain_time == 0) && (g_process[i].end_point == 1)) {
					printf("Process ID : %d 완료\n", g_process[i].pid);
					g_process[i].end_time = time;
					g_process[i].end_point = 0;
					end_cnt -= 1;

					//출력 start
					g_process[i].waiting_time = g_process[i].end_time - (g_process[i].ser_t + g_process[i].ar_t);
					g_process[i].j_time = (g_process[i].waiting_time + g_process[i].ser_t) / (double)g_process[i].ser_t;

					printf("%d\t\t %ds\t\t %ds\t\t %ds\t\t %ds\t\t %.2fs \n\n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t,
						g_process[i].end_time, g_process[i].end_time - g_process[i].ar_t, g_process[i].j_time);
					//출력 End
				}
				//chend


			}

			else if (end_cnt == 0) {
				flag = 0;
			}

		}

	}

	printf("\n\n========================================================================================================================\n\n");


	for (i = 0; i < cnt; i++) {
		if (g_process[i].recoverycnt > 0) {
			printf("ID : %d 카운트발생 %d 회복\n", g_process[i].pid, g_process[i].recoverycnt);
			g_process[i].end_time = g_process[i].end_time - g_process[i].recoverycnt; //역전에 대한 회복
		}
	}
	printf("\n\n========================================================================================================================\n\n");

	for (i = 0; i < cnt; i++) {
		g_process[i].waiting_time = g_process[i].end_time - (g_process[i].ser_t + g_process[i].ar_t);
		g_process[i].j_time = (g_process[i].waiting_time + g_process[i].ser_t) / (double)g_process[i].ser_t;
	}


	for (i = 0; i < cnt; i++) {
		printf("%d\t\t %ds\t\t %ds\t\t %ds\t\t %ds\t\t %.2fs \n\n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t,
			g_process[i].end_time, g_process[i].end_time - g_process[i].ar_t, g_process[i].j_time);
	}
}
int main()
{
	FILE* file = NULL;
	char c[MAX_PROCESS];
	char* a, * b = NULL;

	process copy_sys[MAX_QUEUE] = { 0, };

	int count = 0, i;
	int max_wait_index = 0, one_q = 1, exit = 0;

	memset(g_process, 0, sizeof(g_process));

	//파일 읽어오기
	file = fopen("input.txt", "r");
	if (file == NULL) {
		printf("파일이 존재하지 않습니다.");
		return -1;
	}
	while (fgets(c, MAX_PROCESS, file) != NULL) {
		if (c[0] == '\n') continue;

		a = strtok(c, ",");
		if (*a == '0') {
			break;
		}
		g_process[count].pid = atoi(a);

		a = strtok(NULL, ",");
		g_process[count].ar_t = atoi(a);
		a = strtok(NULL, ",");
		g_process[count].remain_time = g_process[count].ser_t = atoi(a);
		g_process[count].waiting_time = 0;
		count++;

	}
	fclose(file); //파일 닫기
	printf("input.txt\n");
	//input.txt 파일 정보 출력
	for (i = 0; i < count; i++) {
		copy_sys[i] = g_process[i];
		//PID, 도착시간, 수행시간
		printf("%d %d %d \n", g_process[i].pid, g_process[i].ar_t, g_process[i].ser_t);
	}
	puts("===============================================");


	printf("\n<RR 스케줄링>\n\n");
	printf("프로세스 id\t 도착시간\t 서비스 시간\t 종료 시간\t 반환 시간\t 정규화된 반환 시간\n");
	printf("=======================================================================================================\n");
	RR(count, copy_sys);
	printf("=======================================================================================================\n");


}
