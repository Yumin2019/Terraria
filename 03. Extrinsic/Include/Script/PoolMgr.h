#pragma once

/*
Pool을 통하여, 미리 할당을 해둔 Object를 활용하려고 한다.
1. Prototype에서 미리 할당을 해두고... ParticleObject 이런 걸 만들어서... 텍스처 셋팅.. 크기 셋팅을... Position 적당히 만들어둔다.
2. PoolMgr의 Init함수를 통해 이 객체를 생성하여 적당히 배치해놓고.. Enable False 상태로 해둔다.
3. Particle Script에서는 PlayTime을 가지고... 날아가는 처리를 하다가... 시간이 다 되면. Enable = false처리를 하고 Mgr에 자신의 정보를 넣는다.
3-1. 시작하는 상황에서는 Mgr에서 자신의 정보를 빼내고... Enable = true .. 동작하도록 셋팅.

4. 물론 ParticleScript에서는 이러한 처리를 모두 지원할 수 있도록 Init함수 이런 것이 있어야 한다.

자료구조는 어떤 것을 골라야 할까 ?
1. vector 
2. list
3. array

생각해야 하는 핵심은 그거 아니야 ? 지금 일을 하고 있는 Object와 일을 하지 않는 자리를 구분하는 것.
1. 벡터로 한다면..  resize로 20개 정도 잡아두고.. 탈락
2. list로 자신을 없앤다의 개념이 아니지.. 그냥 남아있는 것은 모두 여분이라는 소리니까.
바로 가져다가 일시키면 되고, 보통 n개를 순서대로 일을 시키니까 없애는 처리를 하면되고...
나중에 추가하는 경우에는 단순히 push처리를 하면 된다.
3. array인 경우. 20개의 자리정도를 담아놓고... 사용하면. nullptr넣고. 시키고... 다시 등록되면... nullptr 자리에 자신의 주소를 담고...
반복문을 돌면서 빈 자리를 찾게 된다.

list가 가장 나은 것 같다. 20개 생성해놓고.... 내가 5개가 필요해.. 그러면 vector에 담아서 넘겨주고... 5개를 list에서 날리고... 
처리한 이후에 
*/

enum class LIST_ITEM
{
	WOOD,
	CACTUS,
	GEL,
	END
};

LIST_ITEM GetListItem(ITEM eItem);




class CPoolMgr
{
	SINGLE(CPoolMgr);

private:
	list<class CParticleScript*>		m_ParticleList;
	list<class CItemObjScript*>			m_ObjList[(UINT)LIST_ITEM::END];

	int									m_ObjCount[(UINT)LIST_ITEM::END];

	class CMonsterHPScript*						m_arrMonsterUI[5];

public:
	void OrderParticle(int iCount, vector<CParticleScript*>& vecParticle);
	void OrderObj(LIST_ITEM eItem, int iCount, vector<CItemObjScript*>& vecObj);
	// iCount의 개수만큼 GameObject를 담아서 반환해준다.
	// 이 과정에서 list에서는 iCount만큼의 GameObject를 erase한다. (NO DELETE)

	CItemObjScript* OrderObj(LIST_ITEM eItem); // One Return

	void ReturnParticle(CParticleScript* pParticle); // 일 처리가 끝난 ParticleObj를 하나 반환.
	void ReturnParticle(const vector<CParticleScript*>& vecParticle); // 여러개를 동시에 반환.
	
	void ReturnObj(LIST_ITEM eItem, CItemObjScript* pObj);
	void ReturnObj(LIST_ITEM eItem, const vector<CItemObjScript*>& vecObj);

	// MonsterUI 
	void OrderMonsterUI(class CMonsterScript* pMonster); // 남은 UI를 잡아서 Monster의 체력을 출력하도록 한다. 
	void ReturnMonsterUI(class CMonsterScript* pMonster); // 현재 몬스터를 알고 있는 UI에 접근하여 연결을 끊는다.
public:
	bool Init(int iParticleNum, int arrCount[(UINT)LIST_ITEM::END]);
	// Particle의 개수를 넣어주면... ParticleObject를 Clone하여 담아둔다. 
	// 사용하는 과정에서는 OrderParticle함수를 이용하여 GameObject를 받아가고... 가져다가 사용...
	// 이후에는 Return할 수 있다.

	void SetEnable(bool bEnable); // PoolMgr와 관련된 모든 Object에 대해서 일괄 처리한다.

};

