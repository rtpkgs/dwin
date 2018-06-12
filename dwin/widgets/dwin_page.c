/*
 * @File:   dwin_page.c 
 * @Author: liu2guang 
 * @Date:   2018-06-05 17:27:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-06-05     liu2guang    implementing the page object. 
 */ 

#include "dwin_page.h" 
#include "dwin_system.h" 

struct dwin_page *dwin_page_create(rt_uint16_t id)
{
    struct dwin_page *page = RT_NULL; 
    
    page = (struct dwin_page *)rt_malloc(sizeof(struct dwin_page)); 
    if(page == RT_NULL)
    {
        DWIN_DBG("Create Page failed memory is not enough.\n"); 
        goto failed; 
    }
    
    rt_list_init(&(page->list)); 
    rt_list_init(&(page->objs)); 
    
    page->id = id; 
    
    rt_list_insert_before(&(dwin.pages), &(page->list)); 
    
    dwin.page_num++; 
    
    return page; 
    
failed:
    if(page != RT_NULL)
    {
        rt_free(page); 
    }
    
    return RT_NULL; 
}

rt_err_t dwin_page_delect(struct dwin_page *page)
{
    RT_ASSERT(page != RT_NULL); 
    
    if(dwin_page_current() == page)
    {
        DWIN_DBG("The Page delect failed, this page is current page.\n"); 
        goto failed; 
    }
    
    rt_list_remove(&(page->list)); 
    rt_free(page); 
    
    return RT_EOK; 
    
failed:
    return RT_ERROR; 
}

rt_err_t dwin_page_add_obj(struct dwin_page *page, struct dwin_obj *obj)
{
    RT_ASSERT(page != RT_NULL); 
    RT_ASSERT(obj  != RT_NULL); 
    
    rt_list_insert_before(&(page->objs), &(obj->list)); 
    
    return RT_EOK; 
}

rt_err_t dwin_page_remove_obj(struct dwin_obj *obj)
{
    RT_ASSERT(obj  != RT_NULL); 
    
    rt_list_remove(&(obj->list)); 
    
    return RT_EOK; 
}

struct dwin_page *dwin_page_current(void)
{
    return dwin.page_cur; 
}

struct dwin_page *dwin_page_get_from_id(rt_uint16_t id)
{
    rt_list_t *list = RT_NULL;
    struct dwin_page *page = RT_NULL; 
    
    for(list = dwin.pages.next; list != &(dwin.pages); list = list->next)
    {
        if(rt_list_entry(list, struct dwin_page, list)->id == id)
        {
            page = rt_list_entry(list, struct dwin_page, list);
            break;
        }
    }
    
    return page; 
}

rt_err_t dwin_page_jump(struct dwin_page *page)
{
    RT_ASSERT(page != RT_NULL); 
    
    if(dwin_system_jump(page->id) != RT_EOK)
    {
        DWIN_DBG("Jump %d page failed.\n"); 
        goto failed; 
    }
    
    dwin.page_cur = page; 
    
    return RT_EOK; 
    
failed:
    return RT_ERROR; 
}

rt_err_t dwin_page_jump_id(rt_uint16_t id)
{
    struct dwin_page *page = RT_NULL; 
    
    page = dwin_page_get_from_id(id); 
    if(page == RT_NULL)
    {
        DWIN_DBG("Get %d page struct pointer failed.\n"); 
        goto failed; 
    }
    
    if(dwin_system_jump(id) != RT_EOK)
    {
        DWIN_DBG("Jump %d page failed.\n"); 
        goto failed; 
    }
    
    dwin.page_cur = page; 
    
    return RT_EOK; 
    
failed:
    return RT_ERROR; 
}
