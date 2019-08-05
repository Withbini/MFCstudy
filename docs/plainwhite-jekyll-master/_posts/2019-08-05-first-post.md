---
title: "Cuda video decoder 테스트 버튼 동작 완료 및 C++ 공부 정리, MFC 공부 정리"
date: 2019-08-05
---
initCudaResources 함수부터 다시 분석
===================================
## initCudaResources
CUDA 관련 리소스를 initiate하는 함수
### cuDeviceComputeCapability
GPU 스펙 알려주는 함수
### CUmoduleManager
ptx 파일에서 CUDA 커널함수 갯수, 글로벌 메모리, texture reference array를 받아오는 함수=>정확한건 물어봐야할듯
### initCudaVideo
실제로 비디오 데이터가 들어오는 함수
setParser함수를 통해 framequeue에 있는걸 parsing함
### initD3D9Surface

테스트 버튼 재사용 가능하게 함
==============================
decoding 이후 테스트 버튼을 다시 못 썼는데, 그 이유가 decode loop가 디코딩이 완료되었음에도 계속 루프를 돌고 있었음.
테스트버튼을 누를때마다 새로 스레드를 만들고 load video를 다시 하고 디코드 루프가 돌아가게함

FPS 문제
============
sleep함수가 cpu를 사용하기 때문에 fps 제한을 걸어도, 그 fps가 정확하게 안나왔다.
채널이 1개일 경우 FPSlimit 변수에 맞게 나왔으나, 채널이 16개일 때에는 80FPS로 적었을 때 일반적으로 30정도 나온듯 하다.

C++공부
================
Afxmain에서 virtual 함수가 있기에 상속부터 공부했다.
https://modoocode.com/209 참고하였다.

### 상속 일반론
C++에서는 상속덕분에 C에는 없는 **클래스사이의 관계를 표현**할 수 있다.
상속받을수록(파생되었을 수록) 클래스의 의미가 구체화된다.


~~~C++
#include<iostream>
#include <string>
using namespace std;
class Base{ //Base 클래스
	string s;
	
public:
	Base() : s("기반") { cout << "기반 클래스" << "\n"; }
	void printString(){ cout << s << "\n"; }
};

class Derived :public Base{ //상속
	string s;

public:
	Derived() : Base(), s("파생"){ //상속받을 클래스
		cout << "파생클래스" << '\n';
		printString();
	}
	void printString(){ cout << s << "\n"; }
};

int main(){
	Base b;
	Derived d;
	return 0;
}
~~~
C++의 상속 형태는 다음과 같다.

~~~C++
class [classname] : [상속클래스 접근지시자] [상속class]{}
~~~
이 때 printString()함수는 함수 오버라이딩(overriding)되었다.
오버라이딩은 상속받은 클래스의 함수 하나와 이름이 같지만 내용물이 다른 것을 만들어 낼 수 있다.
즉, 여기에서는 printString()이라는 함수를 class Base에서 찾는것이 아니라 class Derived에서 찾았다.

결과물은 다음과 같다.
~~~C++
기반 클래스
기반 클래스
파생클래스
파생
계속하려면 아무 키나 누르십시오 . . .
~~~

### 업캐스팅
하지만 다음과 같이 할 수도 있다.


~~~C++
int main(){
	//Base b;
	Derived d;
	cout << "===========포인터 버전===========" << "\n";

	Base *p_c = &d;
	p_c->printString();
}
~~~


p_c는 Base의 객체이고 derived 객체를 가리키는데, 이 때 derived가 Base가 상속받은 것이다. **(Derived is Base)**
따라서 p_c는 derived 중 상속받은 Base부분만을 가리키게 되고, p_c->printString()을 하면 Base 클래스의 printString함수를 불러온다.
결과는 다음과 같다.


~~~C++
기반 클래스
파생 클래스
===========포인터 버전===========
기반
계속하려면 아무 키나 누르십시오 . . .
~~~

### protected
클래스 멤버에 접근 지시자를 정하지 않으면 private으로 된다. **즉, 자기 클래스 외에는 접근할 수 없다.**
protected 접근 지시자를 사용하면 상속받는 클래스에서는 접근 가능하고 그 외의 기타 정보는 접근 불가능

상속클래스 접근지시자에 따라
* public :원래 접근지시자 유지
* protected : public이 protected로 바뀜
* private : 모든 접근지시자가 private로 바뀜


### Virtual 키워드

*귀찮아서 그대로 복붙*
*what함수=printString함수이다*


~~~C++
#include <iostream>

class Base {

 public:
  Base() { std::cout << "기반 클래스" << std::endl; }

  virtual void what() { std::cout << "기반 클래스의 what()" << std::endl; }
};
class Derived : public Base {

 public:
  Derived() : Base() { std::cout << "파생 클래스" << std::endl; }

  void what() { std::cout << "파생 클래스의 what()" << std::endl; }
};

int main() {
  Base p;
  Derived c;

  Base* p_c = &c;
  Base* p_p = &p;

  std::cout << " == 실제 객체는 Base == " << std::endl;
  p_p->what();

  std::cout << " == 실제 객체는 Derived == " << std::endl;
  p_c->what();

  return 0;
}
~~~


위와 같이 Base 클래스의 what함수에 virtual 키워드를 적어주면 결과는 다음과 같다.

~~~C++
기반 클래스
기반 클래스
파생 클래스
 == 실제 객체는 Base == 
기반 클래스의 what()
 == 실제 객체는 Derived == 
파생 클래스의 what()
~~~

p_c가 derived를 가리키는 Base객체이므로 base에 있는 what()함수를 가져왔으나, virtual 키워드를 통해서 **동적 바인딩**으로 
런타임동안에 어떤 함수를 사용하는지 결정된다.

**덧붙여서 C++11이상에서, 파생클래스 함수에 'override' 키워드를 사용한다면 overide 여부를 컴파일 타임에 알 수 있다.**
**이를 다형성(polymorphism)이라고 한다**
polymorphism-하나의 메소드 호출이지만 각자 다른 역할을 한다는 의미

~~AFX 공부
==========================
이제 virtual도 공부했으니, Afx로 들어가자

~~MFC는 일반적인 라이브러리와 달리 프레임워크 기반의 라이브러리이다.
MFC는 윈도우즈 어플리케이션으로 처리해야 할 대부분의 논리를 MFC 내부적(AfxWinMain 부터 시작)으로 구현해 두고 있다. 하지만, MFC는 파일 구조상 실행 파일이 아니라 DLL이기 때문에 단독으로 실행될 수 없다. 따라서 반드시 한번은 실행 어플리케이션에 의해 호출되어야만 프레임웍으로 동작을 시작할 수 있는 것이다. 그래서 숨겨진 WinMain()를 통해 개발자가 다른 실행 흐름을 선택할 수 없도록 막아 버리면서 AfxWinMain()가 강제적으로 호출되도록 유도하기 위하여 이런 구조를 사용하는 것이다. 만약, WinMain()가 어플리케이션 코드에 노출되어 있다면 개발자가 직접 WinMain을 다르게 구현할 수 도 있을 것이다. 그렇게 되면 MFC 내부의 AfxWinMain()가 호출될 수 없으므로 MFC 프레임웍이 시작될 수 없게된다.

~~출처: https://petra.tistory.com/1296 [Free, Freedom]

~~### 내 언어로 정리해보자
MFC는 윈도우즈 어플리케이션 처리부분을 MFC 내부적으로 처리한다.
이 때 MFC가 dll이 파일이므로 혼자서 실행될 수 없고, 실행 어플리케이션에 의해 호출되어야 한다.
Afxwinmain으로 다른 실행을 할 수 없게 강제적으로 호출되도록 한다.~~

위 블로그는 내가 이해하기에 너무 어려워서 패스


win32 이해하기 & MFC 도입배경
===========
###windows 프로그램 이해하기

windows 프로그램은 windows main과 windows procedure로 나뉘어 다음과 같은 구조를 가진다.

~~~C++
WinMain( )
{
       윈도우 클래스 만들기
       윈도우 객체 생성하기
       윈도우 객체 화면에 띄우기
       메시지 루프 돌리기
}


WndProc( )
{
       전달된 메시지 처리하기
}
~~~~

윈도우 메인에서는 윈도우 객체를 만들고 메시지 루프를 돌린다.
그리고 메시지가 들어올 때마다 각 응용프로그램의 메시지 큐(어플리케이션 큐)에 해당 내용을 전달한다.
그러면 윈도우 프로시저가 해당 메시지에 대한 내용을 처리한다.

다음은 하얀 윈도우 바탕 응용프로그램이다.
~~~C++
#include <Windows.h>
#include <tchar.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPTSTR lpszClass = _T("BasicApi");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;


	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);


	//윈도우 객체 생성
	hWnd = CreateWindow(lpszClass, LPTSTR(_T("win32")), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	//메시지
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message); //문자입력 처리함수
		DispatchMessage(&Message); //GetMessage함수로부터 전달된 메시지를 winproc에 보내는 역할
	}

	return Message.wParam;

}


//hwnd 메시지 받을 윈도우 핸들
//iMessage 전달된 메시지
//wParam,lParam iMessage에 따른 부가정보 
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int nReturn;
	//메시지 형태에 따라 이벤트 다르게 해야하므로 switch문
	switch (iMessage)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_CLOSE:

		nReturn = MessageBox(hWnd, _T("정말 종료하시겠습니까?"), _T("확인"), MB_YESNO);
		if (nReturn == IDYES)
			DestroyWindow(hWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}
	//case문에서 처리되지 않은 메시지는 DefWindowProc로 가 메시지 처리
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));

}
~~~

문제는 위와 같이 윈도우 관련 API들이 한두가지가 아니라는 것이다.
그렇다보니, MS에서 관련 API끼리 모아 클래스를 만들었고 그것이 Microsoft Foundation Class(MFC)이다.
물론 현재에는 지원을 중단한 것 같고, 이후 Video decoder 코드도 시간여유가 된다면 .NET으로 갈아탈 예정이다.


MFC
======
MFC는 먼저 소스파일과 리소스파일로 구성된다.
소스파일은 .obj로 컴파일 되고, 리소스파일은 .rc로 컴파일되어서
이들이 링크되어서 하나의 Excution file이 된다.

리소스파일은 응용 프로그램에서 사용하는 메뉴, 대화 상자, 아이콘, 비트맵, 단축키 정보 등 소스 코드 이외의 데이터를 의미한다.[LINK]https://thebook.io/006796/ch02/02/03_01/

### 리소스뷰 분석
리소스뷰에서 관련된 항목들부터 분석해보자

![resourceview](withbini.github.io/img/resourceview.jpg)
