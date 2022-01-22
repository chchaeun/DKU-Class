# OS-Lock-Simulator

## 문제 정의
### 문제 조건
- 5종류의 차량
- 하나의 생산 라인: Round Robin 방식으로 출고 진행(Time quantum = 1, 4)
- 생산 라인에 출고된 차량이 없는 경우 차량을 가져갈 수 없고, 생산 라인이 가득 찬 경우 새롭게
차량을 출고 할 수 없음
### 주요한 프로그램 구조
- Vehicle 구조체: 차량 id, 생산 수 포함
- Queue 구조체: fill_ptr, use_ptr, buf[]로 원형 큐 구현. Lock 구현을 위해 fillLock, useLock이라는
mutex lock 변수 선언
- Producer/Consumer 함수: balance가 0부터 MAX 사이에서 증가/감소하도록 조건변수를 이용하여
Lock을 걸어주어야 한다.

## 프로그램 설계
### 파일 목록
 - include: 헤더파일 포함
 - lab2_sync.c: 프로그램 구현

## 프로그램 구현
### Workload 설정
|차량 ID|생산 수|
|:---:|:---:|
|1|전체 차량의 10%|
|2|전체 차량의 20%|
|3|전체 차량의 20%|
|4|전체 차량의 20%|
|5|전체 차량의 30%|

### Round Robin
큐에 차량 id를 추가할 때마다 round 변수를 증가해주고, 각 차량의 생산 수인 prodNum을 감소시
킨다. round가 TimeQt과 같아지거나, idx번째 차량의 prodNum이 0이 되어 더 이상 출고할 차량이 없
으면 차량의 종류를 변경한다. idx = (idx+1)%NUM; 코드는 NUM의 범위 안에서 idx를 바꿔줄 수 있
다.
### Producer/Consumer
전체 차량 수만큼 스레드를 생성하고, producer()와 consumer()를 호출한다. producer()는 put()을 호
출해서 큐에 차량 id를 추가해주고, balance를 1 증가시킨다. consumer()는 get()을 호출하여 큐의 맨
앞에 있는 차량 id를 삭제하고, balance를 1 감소시킨다.
### Locking
- Coarse-grained Lock
lock 변수가 2이면 Coarse-grained Lock을 수행한다. producer()와 consumer()에서 사용되는 임계영
역인 balance와 queue를 한 번에 lock 해주었다. 조건변수를 사용하여 producer()는 balance가 MAX
일 때, consumer()는 balance가 0일 때 pthread_cond_wait()를 걸어준다.
- Fine-grained Lock
lock 변수가 3이면 Fine-grained Lock을 수행한다. producer()와 consumer()에서 사용되는 임계영역
인 balance와 queue를 나눠서 lock 해주었다. mutex lock 변수 balanceLock를 선언하고, balance를 증
가하거나 감소할 때 lock을 걸어준다.
큐의 fill_ptr과 use_ptr를 변경해줄 때도 각각 lock을 걸어주었다. 큐 안에 mutex lock 변수를 선언
해주고, fill_ptr와 use_ptr를 사용할 때 걸어준다.
