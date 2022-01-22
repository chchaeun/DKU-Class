# OS-Scheduling-Simulator

## 프로그램 구조
### 파일 목록
- include: 헤더파일 포함
- lab1_sched.c: 스케줄러 구현
- lab1_sched.test.c: 스케줄러 결과 출력
- make file: 미리 정의된 make file. 실행 시 lab1_sched.c와 lab1_sched_test.c를 gcc 컴파일

## 프로그램 설명
### FIFO
FIFO 스케줄링 기법은 Arrival time을 최우선으로 두고 스케줄링하는 기법이다. 먼저 프로세스를
Arrival time을 기준으로 정렬해준다. 정렬된 프로세스의 pid를 큐에 차례대로 넣어준다. FIFO는
Service time과 관계없이 Arrival time이 가장 빠른 것을 최우선으로 스케줄링하기 때문에, 큐에 들
어간 순서대로 스케줄링해준다. stateArr는 x축이 시간, y축이 프로세스인 테이블이다. 프로세스가
READY 상태인지, RUN 상태인지 배열에 추가해준다.
### SFJ
SFJ 스케줄링 기법은 Service time이 짧은 프로세스를 우선적으로 스케줄링한다. Service time을
기준으로 프로세스를 정렬해주고 큐에 넣는다. Arrival time을 고려하여 큐에 프로세스의 pid를 넣
어준다. resultPrint를 위해 queue에 들어있는 순서대로 프로세스를 다시 정렬해준다.
### RR
RR 스케줄링 기법은 Time quantum을 고려하여 프로세스를 계속해서 전환하는 FIFO 스케줄링
기법이라고 할 수 있다. 실행할 때 Time quantum이 달라지는 것을 고려하여 큐에서 front를 추출
해서 stateArr를 설정할 때, 큐를 쌓아두고 Time Quantum 주기마다 stateArr에 RUN 상태를 추가
해주었다.
