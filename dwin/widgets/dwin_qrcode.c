/*
 * @File:   dwin_qrcode.c
 * @Author: balanceTWK
 * @Date:   2018-06-15 14:00:00
 *
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-06-15     balanceTWK    implementing the button object.
 */

#include "dwin_qrcode.h"
#include "dwin_obj.h"
#include "dwin_page.h"
#include "dwin_trans.h"
#include "string.h"

rt_err_t dwin_qrcode_show_url(struct dwin_qrcode *qrcode, char *url)
{
    RT_ASSERT(qrcode != RT_NULL);
    RT_ASSERT(url != RT_NULL);

    rt_uint16_t data[2] = {0};
    rt_uint8_t index = 4;
    rt_uint16_t addr = qrcode->obj.value_addr;

    dwin_var_read(addr, data, 2);
    url[0] = (char)(data[0] >> 8);
    url[1] = (char)data[0];
    url[2] = (char)(data[1] >> 8);
    url[3] = (char)data[1];
    url[4] = (char)0x00;
    addr += 2;
    if ((!strcmp(url, "http")) || (!strcmp(url, "HTTP")) || (!strcmp(url, "WWW.")) || (!strcmp(url, "www.")))
    {
        for (rt_uint8_t i = 0; i <= 64; i++)
        {
            dwin_var_read(addr, data, 2);
            addr += 2;
            url[index] = (char)(data[0] >> 8);
            url[index + 1] = (char)data[0];
            url[index + 2] = (char)(data[1] >> 8);
            url[index + 3] = (char)data[1];
            for (rt_uint8_t x = 0; x < 4; x++)
            {
                if ((url[index + x] == (char)0xff) || (url[index + x] == (char)0x00))
                {
                    url[index + x] = 0x00;
                    DWIN_DBG("url: %s \n", url);
                    return RT_EOK;
                }
            }
            index += 4;
        }
    }
    else
    {
        DWIN_DBG("no network address found .\n");
        return RT_ERROR;
    }
}

rt_err_t dwin_qrcode_set_url(struct dwin_qrcode *qrcode, char *url)
{
    RT_ASSERT(qrcode != RT_NULL);
    RT_ASSERT(url != RT_NULL);

    rt_uint16_t len = 0;
    len = strlen(url);
    len = len / 2 + len % 2;
    rt_uint16_t data[len];

    for (rt_uint16_t i = 0; i < len; i++)
    {
        data[i] = (rt_uint16_t)(url[i * 2] << 8) + (rt_uint16_t)(url[i * 2 + 1]);
    }

    dwin_var_write(qrcode->obj.value_addr, data, len);
    return RT_EOK;
}
