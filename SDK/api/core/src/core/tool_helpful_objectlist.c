#include <core/tools/tool_helpful.h>

void th_object_list_reset(Pth_objectlist pqlist)
{
	memset(pqlist, 0, sizeof(th_objectlist));
}

int32_t th_object_list_exists(Pth_objectlist pqlist, void* pobj)
{
	Pth_objectlink plink = pqlist->root;
	while (plink)
	{
		if (plink->qobject == pobj)
			return 1;
		plink = plink->next;
	}
	return 0;
}

void th_object_list_push(Pth_mempool pmemp, Pth_objectlist plist, void* pobj)
{
	Pth_objectlink plink = plist->root;
	while (plink)
	{
		if (plink->qobject == pobj)
		{
			plink->ref++;
			return;
		}
		plink = plink->next;
	}

	plink = (Pth_objectlink)th_mempool_malloc(pmemp, sizeof(th_objectlink));
	plink->qobject = pobj;
	plink->ref = 1;
	if (!plist->root)plist->root = plink;
	else plist->last->next = plink;
	plist->last = plink;
	plist->nums++;
}

thobj_position th_object_list_get_first(Pth_objectlist plist)
{
	if (plist->root)
		return plist->root;
	return NULL;
}

void* th_object_list_get_next(thobj_position* pos)
{
	void* pval = ((Pth_objectlink)*pos)->qobject;
	*pos = ((Pth_objectlink)*pos)->next;
	return pval;
}

#if 0

void* qobject_list_get_first(Pqobject_list plist)
{
	plist->ptr = plist->root;

	if (plist->ptr)
		return plist->ptr->qobject;
	return NULL;
}

void* qobject_list_get_next(Pqobject_list plist)
{
	if (plist->ptr)
		plist->ptr = plist->ptr->next;

	if (plist->ptr)
		return plist->ptr->qobject;
	return NULL;
}

#endif
