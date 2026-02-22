#pragma once

//레퍼런스 카운트 기능을 정의한 클래스
class CRef
{
public:
	CRef() {};
	virtual ~CRef() {};


	//CRef클래스에는 기본 타입인 int만 있으면 복사함수는 컴파일러가 만들어주는걸로 사용.

public:
	inline void AddRef() {
		++mRef;
	}

	//참조 카운팅 감소,
	inline int Release() {
		--mRef;

		//최종적으로 참조 카운트가 0이 되면, 동적할당 메모리 삭제
		if (mRef == 0)
		{
			delete this;
			return 0;
		}

		return mRef;
	}
protected:
	int mRef = 0; //참조 카운팅 변수
};

