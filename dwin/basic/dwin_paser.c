/*
 * @File:   dwin_paser.c 
 * @Author: liu2guang 
 * @Date:   2017-12-08 15:00:00 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2017-12-08     liu2guang    实现插件动作同步解析, 解析会占用上传数据时间, 
 *                             所以插件的回调不好去处理耗时动作. 
 */ 
#include "list.h" 
#include "dwin_paser.h" 
#include "dwin_space.h" 

/* 导入空间变量器链表头 */
extern list_t *dwin_space_list;

/* 自动上传数据命令解析 */
uint8_t dwin_paser(uint8_t *data, uint8_t len)
{
    list_node_t *node = RT_NULL;
    list_iterator_t *iterator = RT_NULL;
    dwin_space_t space = RT_NULL;
    
    /* 判断数据帧是否是合法的 */
    if(data[3] != DWIN_VAR_READ)
    {
        dwin_println("dwin paser data error");
        return dwin_err_error;
    }
    
    /* 申请迭代器 */
    iterator = list_iterator_new(dwin_space_list, LIST_HEAD);
    
    /* 迭代器遍历列表 */
    while((node = list_iterator_next(iterator)) != RT_NULL)
    {
        space = (dwin_space_t)(node->val);
        
        /* 地址匹配 and 长度匹配 */
        if(space->addr == ((data[4]<<8)+(data[5])) && space->len == data[6])
        {
            switch(space->type)
            {
#ifdef PKG_DWIN_ENABLE_PLUGIN_BUTTON
                /* 按键插件 */
                case DWIN_TYPE_BTN:
                {
                    dwin_button_t button = (dwin_button_t)(space->plugin);
                    
                    /* 未注册按键回调函数 */
                    if(button->press_cb == RT_NULL)
                    {
                    #if (PKG_DWIN_DEBUG >= 2)
                        dwin_println("Button %s no callback func", space->name);
                    #endif
                        break;
                    }
                    
                    /* 按键匹配判断 */
                    if(button->match_value == ((data[7]<<8)+data[8]))
                    {
                        switch(button->state)
                        {
                            /* 按键启动状态 */
                            case button_press:
                            {
                                /* 回调函数 */
                                button->press_cb(button->args);
                            }
                            break;
                            
                            /* 按键停止状态 */
                            case button_stop:
                            {
                            #if (PKG_DWIN_DEBUG >= 2)
                                dwin_println("Button %s stop state", space->name);
                            #endif
                            }
                            break;
                        }
                    }
                }
                break;
#endif
                
                /* 输入框插件 */
#ifdef PKG_DWIN_ENABLE_PLUGIN_INPUTBOX
                case DWIN_TYPE_INPUTBOX:
                {
                    dwin_inputbox_t inputbox = (dwin_inputbox_t)(space->plugin);
                    
                    /* 未注册按键回调函数 */
                    if(inputbox->inputbox_cb == RT_NULL)
                    {
                    #if (PKG_DWIN_DEBUG >= 2)
                        dwin_println("Inputbox %s no callback func", space->name);
                    #endif
                        break;
                    }
                    
                    switch(inputbox->state)
                    {
                        case inputbox_start:
                        {
                            /* 获取输入值 */
                            inputbox->input_value = (data[7]<<24)+(data[8]<<16)+(data[9]<<8)+data[10];
                        #if (PKG_DWIN_DEBUG >= 2)
                            dwin_println("Inputbox %s input %d", space->name, inputbox->input_value);
                        #endif
                            
                            /* 回调函数 */
                            inputbox->inputbox_cb(inputbox->input_value, inputbox->args);
                        }
                        break;
                        
                        /* 按键停止状态 */
                        case inputbox_stop:
                        {
                        #if (PKG_DWIN_DEBUG >= 2)
                            dwin_println("Inputbox %s stop state", space->name);
                        #endif
                        }
                        break;
                    }
                }
                break;
#endif
            }
        }
    }
    
    list_iterator_destroy(iterator);
    
    return dwin_err_none;
}
