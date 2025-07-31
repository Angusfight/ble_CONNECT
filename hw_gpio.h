#ifndef __HW_GPIO_H__
#define __HW_GPIO_H__

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

int hw_gpio_export(uint32_t gpio);
int hw_gpio_unexport(uint32_t gpio);
int hw_gpio_set_dir(uint32_t gpio, uint32_t out_flag);
int hw_gpio_set_value(uint32_t gpio, uint32_t value);
int hw_gpio_get_value(uint32_t gpio, uint32_t *value);
int hw_gpio_set_edge(uint32_t gpio, char *edge);
int hw_gpio_fd_open(uint32_t gpio);
int hw_gpio_fd_close(int fd);


#ifdef __cplusplus
}
#endif



#endif /* _HW_GPIO_H_ */
