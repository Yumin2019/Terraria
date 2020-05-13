#pragma once

/*
Pool�� ���Ͽ�, �̸� �Ҵ��� �ص� Object�� Ȱ���Ϸ��� �Ѵ�.
1. Prototype���� �̸� �Ҵ��� �صΰ�... ParticleObject �̷� �� ����... �ؽ�ó ����.. ũ�� ������... Position ������ �����д�.
2. PoolMgr�� Init�Լ��� ���� �� ��ü�� �����Ͽ� ������ ��ġ�س���.. Enable False ���·� �صд�.
3. Particle Script������ PlayTime�� ������... ���ư��� ó���� �ϴٰ�... �ð��� �� �Ǹ�. Enable = falseó���� �ϰ� Mgr�� �ڽ��� ������ �ִ´�.
3-1. �����ϴ� ��Ȳ������ Mgr���� �ڽ��� ������ ������... Enable = true .. �����ϵ��� ����.

4. ���� ParticleScript������ �̷��� ó���� ��� ������ �� �ֵ��� Init�Լ� �̷� ���� �־�� �Ѵ�.

�ڷᱸ���� � ���� ���� �ұ� ?
1. vector 
2. list
3. array

�����ؾ� �ϴ� �ٽ��� �װ� �ƴϾ� ? ���� ���� �ϰ� �ִ� Object�� ���� ���� �ʴ� �ڸ��� �����ϴ� ��.
1. ���ͷ� �Ѵٸ�..  resize�� 20�� ���� ��Ƶΰ�.. Ż��
2. list�� �ڽ��� ���ش��� ������ �ƴ���.. �׳� �����ִ� ���� ��� �����̶�� �Ҹ��ϱ�.
�ٷ� �����ٰ� �Ͻ�Ű�� �ǰ�, ���� n���� ������� ���� ��Ű�ϱ� ���ִ� ó���� �ϸ�ǰ�...
���߿� �߰��ϴ� ��쿡�� �ܼ��� pushó���� �ϸ� �ȴ�.
3. array�� ���. 20���� �ڸ������� ��Ƴ���... ����ϸ�. nullptr�ְ�. ��Ű��... �ٽ� ��ϵǸ�... nullptr �ڸ��� �ڽ��� �ּҸ� ���...
�ݺ����� ���鼭 �� �ڸ��� ã�� �ȴ�.

list�� ���� ���� �� ����. 20�� �����س���.... ���� 5���� �ʿ���.. �׷��� vector�� ��Ƽ� �Ѱ��ְ�... 5���� list���� ������... 
ó���� ���Ŀ� 
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
	// iCount�� ������ŭ GameObject�� ��Ƽ� ��ȯ���ش�.
	// �� �������� list������ iCount��ŭ�� GameObject�� erase�Ѵ�. (NO DELETE)

	CItemObjScript* OrderObj(LIST_ITEM eItem); // One Return

	void ReturnParticle(CParticleScript* pParticle); // �� ó���� ���� ParticleObj�� �ϳ� ��ȯ.
	void ReturnParticle(const vector<CParticleScript*>& vecParticle); // �������� ���ÿ� ��ȯ.
	
	void ReturnObj(LIST_ITEM eItem, CItemObjScript* pObj);
	void ReturnObj(LIST_ITEM eItem, const vector<CItemObjScript*>& vecObj);

	// MonsterUI 
	void OrderMonsterUI(class CMonsterScript* pMonster); // ���� UI�� ��Ƽ� Monster�� ü���� ����ϵ��� �Ѵ�. 
	void ReturnMonsterUI(class CMonsterScript* pMonster); // ���� ���͸� �˰� �ִ� UI�� �����Ͽ� ������ ���´�.
public:
	bool Init(int iParticleNum, int arrCount[(UINT)LIST_ITEM::END]);
	// Particle�� ������ �־��ָ�... ParticleObject�� Clone�Ͽ� ��Ƶд�. 
	// ����ϴ� ���������� OrderParticle�Լ��� �̿��Ͽ� GameObject�� �޾ư���... �����ٰ� ���...
	// ���Ŀ��� Return�� �� �ִ�.

	void SetEnable(bool bEnable); // PoolMgr�� ���õ� ��� Object�� ���ؼ� �ϰ� ó���Ѵ�.

};

