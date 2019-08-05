---
title: "Cuda video decoder 완료"
date: 2019-08-05 18:26:28 -0400
categories: jekyll update
---
#initCudaResources 함수부터 다시 분석
##initCudaResources 
cuDeviceComputeCapability
GPU 스펙 알려주는 함수
CUmoduleManager
ptx 파일에서 CUDA 커널함수 갯수, 글로벌 메모리, texture reference array를 받아오는 함수=>정확한건 물어봐야할듯
initCudaVideo
실제로 비디오 데이터가 들어오는 함수
setParser함수를 통해 framequeue에 있는걸 parsing함
initD3D9Surface

테스트 버튼 재사용 가능하게 함
decoding 이후 테스트 버튼을 다시 못 썼는데, 그 이유가 decode loop가 디코딩이 완료되었음에도 계속 루프를 돌고 있었음.
테스트버튼을 누를때마다 새로 스레드를 만들고 load video를 다시 하고 디코드 루프가 
돌아가게함

FPS 문제
sleep함수가 cpu를 사용하기 때문에 fps 제한을 걸어도, 그 fps가 정확하게 안나왔다.

