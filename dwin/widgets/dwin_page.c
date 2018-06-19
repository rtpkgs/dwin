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
    RT_ASSERT(page != DWIN_ALL_PAGE); 
    
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
    
    if(page == DWIN_ALL_PAGE)
    {
        rt_list_insert_before(&(dwin.global_objs), &(obj->list)); 
    }
    else
    {
        rt_list_insert_before(&(page->objs), &(obj->list)); 
    } 
    
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
    RT_ASSERT(page != DWIN_ALL_PAGE); 
    
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

/* 打印已经注册控件信息 */ 
void dwin_page_obj_info(struct dwin_page *page)
{
    extern const char *widgets_info[]; 
    rt_list_t *list = RT_NULL; 
    struct dwin_obj *obj = RT_NULL; 
    
    /* 遍历全局控件 */ 
    for(list = dwin.global_objs.next; list != &(dwin.global_objs); list = list->next)
    {
        obj = rt_list_entry(list, struct dwin_obj, list); 
        
        if(obj->active)
        {
            if(obj->value_size == DWIN_OBJ_VARY_LENGHT)
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size VaryLen, active enable, \033[32mGlobal\033[0m.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
            else
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size %d, active enable, \033[32mGlobal\033[0m.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
        }
        else
        {
            if(obj->value_size == DWIN_OBJ_VARY_LENGHT)
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size VaryLen, active disable, \033[32mGlobal\033[0m.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
            else
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size %d, active disable, \033[32mGlobal\033[0m.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
        }
    }
    
    /* 遍历当前页面所有控件 */ 
    for(list = page->objs.next; list != &(page->objs); list = list->next)
    {
        obj = rt_list_entry(list, struct dwin_obj, list); 
        
        if(obj->active)
        {
            if(obj->value_size == DWIN_OBJ_VARY_LENGHT)
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size VaryLen, active enable.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
            else
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size %d, active enable.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
        }
        else
        {
            if(obj->value_size == DWIN_OBJ_VARY_LENGHT)
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size VaryLen, active disable.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
            else
            {
                DWIN_PRINT("\tObj: type %s, addr 0x%.4x, size %d, active disable.\n", widgets_info[obj->type], obj->value_addr, obj->value_size); 
            }
        }
    }
}

/* 打印已经创建注册页面信息 */ 
void dwin_page_info(void)
{
    rt_list_t *list = RT_NULL; 
    struct dwin_page *page = RT_NULL; 
    
    for(list = dwin.pages.next; list != &(dwin.pages); list = list->next)
    {
        page = rt_list_entry(list, struct dwin_page, list); 
        
        DWIN_INFO("Page: id %.4d.\n", page->id); 
        dwin_page_obj_info(page); 
    }
}
