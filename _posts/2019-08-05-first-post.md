---
title: "Cuda video decoder 완료"
date: 2019-08-05 18:26:28 -0400
---
initCudaResources 함수부터 다시 분석
===================================
initCudaResources
-------------------
### cuDeviceComputeCapability
GPU 스펙 알려주는 함수
### CUmoduleManager
ptx 파일에서 CUDA 커널함수 갯수, 글로벌 메모리, texture reference array를 받아오는 함수=>정확한건 물어봐야할듯
# initCudaVideo
실제로 비디오 데이터가 들어오는 함수
setParser함수를 통해 framequeue에 있는걸 parsing함
# initD3D9Surface

테스트 버튼 재사용 가능하게 함
==============================
decoding 이후 테스트 버튼을 다시 못 썼는데, 그 이유가 decode loop가 디코딩이 완료되었음에도 계속 루프를 돌고 있었음.
테스트버튼을 누를때마다 새로 스레드를 만들고 load video를 다시 하고 디코드 루프가 
돌아가게함

FPS 문제
============
sleep함수가 cpu를 사용하기 때문에 fps 제한을 걸어도, 그 fps가 정확하게 안나왔다.

AFX 공부
==========================
MFC는 일반적인 라이브러리와 달리 프레임워크 기반의 라이브러리이다.
MFC는 윈도우즈 어플리케이션으로 처리해야 할 대부분의 논리를 MFC 내부적(AfxWinMain 부터 시작)으로 구현해 두고 있다. 하지만, MFC는 파일 구조상 실행 파일이 아니라 DLL이기 때문에 단독으로 실행될 수 없다. 따라서 반드시 한번은 실행 어플리케이션에 의해 호출되어야만 프레임웍으로 동작을 시작할 수 있는 것이다. 그래서 숨겨진 WinMain()를 통해 개발자가 다른 실행 흐름을 선택할 수 없도록 막아 버리면서 AfxWinMain()가 강제적으로 호출되도록 유도하기 위하여 이런 구조를 사용하는 것이다. 만약, WinMain()가 어플리케이션 코드에 노출되어 있다면 개발자가 직접 WinMain을 다르게 구현할 수 도 있을 것이다. 그렇게 되면 MFC 내부의 AfxWinMain()가 호출될 수 없으므로 MFC 프레임웍이 시작될 수 없게된다.
출처: https://petra.tistory.com/1296 [Free, Freedom]

# 내 언어로 정리해보자
MFC는 윈도우즈 어플리케이션 처리부분을 MFC 내부적으로 처리한다.
이 때 MFC가 dll이 파일이므로 혼자서 실행될 수 없고, 실행 어플리케이션에 의해 호출되어야 한다.
Afxwinmain으로 다른 실행을 할 수 없게 강제적으로 호출되도록 한다.
