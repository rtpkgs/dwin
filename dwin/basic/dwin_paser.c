#include "dwin_paser.h" 
#include "dwin_space.h" 
#include "list.h" 

#include "dwin_plugin_button.h" 
#include "dwin_plugin_inputbox.h" 
#include "dwin_plugin_icon.h" 

extern list_t *dwin_space_list;

/* 导出api */
/* 自动上传数据命令解析 */
uint8_t dwin_paser(uint8_t *data, uint8_t len)
{
    list_node_t *node = RT_NULL;
    list_iterator_t *iterator = RT_NULL;
    
    /* 判断数据帧是否是合法的 */
    if(data[3] != DWIN_VAR_READ)
    {
        dwin_println("dwin paser data error");
        return dwin_err_error;
    }
    
    iterator = list_iterator_new(dwin_space_list, LIST_HEAD);
    
    /* 迭代器遍历列表 */
    while((node = list_iterator_next(iterator)) != RT_NULL)
    {
        /* 地址匹配 and 长度匹配 */
        if(((dwin_space_t)(node->val))->addr == ((data[4] << 8) + (data[5])) && 
           ((dwin_space_t)(node->val))->len  == data[6])
        {
            switch(((dwin_space_t)(node->val))->type)
            {
                case dwin_type_button:
                {
                    /* 判断是否注册了回调函数 */
                    if(((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->press_cb != RT_NULL && 
                       ((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->match_value == ((data[7]<<8)+data[8]))
                    {
                        switch(((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->state)
                        {
                            case button_press:
                            {
                                ((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->press_cb(((dwin_button_t)(((dwin_space_t)(node->val))->plugin))->args);
                            }
                            break;
                        }
                    }
                }
                break;
                
                case dwin_type_inputbox:
                {
                    if(((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->inputbox_cb != RT_NULL)
                    {
                        switch(((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->state)
                        {
                            case inputbox_start:
                            {
                                ((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->input_value = (data[7]<<24) + (data[8]<<16) + (data[9]<<8) + data[10];
                                ((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->inputbox_cb(((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->input_value, ((dwin_inputbox_t)(((dwin_space_t)(node->val))->plugin))->args);
                            }
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    
    list_iterator_destroy(iterator);
    
    return dwin_err_none;
}
