#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <utility>
#include <string.h>
using namespace std;
int arr[2][200001];

bool visited[2][200001];
bool ans = false;
int dx[] = { 1, -1, 0, 0 };
int dy[] = { 0, 0, 1, -1 };
queue<pair<pair<int, int>, int >> q;
int n, k;
int bfs() {
	// (왼,오), y위치, while 끝났을 때 사라질 위치
	q.push({ {0,1 }, 1 });
	visited[0][1] = 1;
	while (!q.empty() && ans==false) {
		int x = q.front().first.first;
		int y = q.front().first.second;

		int cnt = q.front().second;
		
		q.pop();

		//현 위치가 n보다 크거나 같으면 탈출
		if (y >= n && cnt-1<y) {
			ans = true;
			break;
		}
		
		//현재 이동 불가능한 위치
		if (cnt-1>=y) break;
		//탈출

		for (int i = 0; i < 2; i++)
		{
			int nx = x + dx[i];
			int ny = y + dy[i]+k;
			
			
			//다음 위치가 주어진 n보다 작고 다음에 사라질 애보다 크면
			if (nx >= 0 && ny > cnt && nx < 2)
			{
				//다음 위치가 안전한 칸이면서 방문한적 없으면 방문
        //이유는 모르겠지만 n보다 큰 arr는 167843009이라는 말도 안되는 값으로 초기화되어서 그거때문에 >=1해야함 -
				if (arr[nx][ny] >= 1 && visited[nx][ny] == false)
				{
					visited[nx][ny] = true;
					q.push(make_pair(make_pair(nx, ny), cnt + 1));
				}
			}
		}

		for (int i = 2; i < 4; i++)
		{
			int nx = x + dx[i];
			int ny = y + dy[i];

			if (nx >= 0 && ny > cnt && nx < 2)
			{
				if (arr[nx][ny] >= 1 && visited[nx][ny] == false)
				{
					visited[nx][ny] = true;
					q.push(make_pair(make_pair(nx, ny), cnt + 1));
				}
			}
		}

		
	}
	//게임 클리어할 수 있으면 1리턴
	if (ans == true) return 1;
	else return 0;
}

int main() {
	for (int i = 0; i < 2; i++) {
		memset(arr[i], (int)1, sizeof(int) * 200001);
	}
	cin >> n >> k;
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j <= n; j++) {
			scanf("%1d", &arr[i][j]);
		}
	}
	

	
	cout <<bfs();
	return 0;
}
