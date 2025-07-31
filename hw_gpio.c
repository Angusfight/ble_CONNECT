/* Copyright (c) 2011, RidgeRun
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hw_gpio.h"



//****************************************************************
//* Constants
//****************************************************************
#define SYSFS_GPIO_DIR "/sys/class/gpio"


#define POLL_TIMEOUT (3*1000) /* 3 seconds */
#define MAX_BUF 64


//----------------------------------------------------------------
// gpio_export
//----------------------------------------------------------------
int hw_gpio_export(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];
    int status;

    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }
    len = snprintf(buf, sizeof(buf), "%d", gpio);
    status = write(fd, buf, len+1);
    close(fd);

    return 0;
}

//----------------------------------------------------------------
// gpio_unexport
//----------------------------------------------------------------
int hw_gpio_unexport(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];
    int status;

    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(buf, sizeof(buf), "%d", gpio);
    status = write(fd, buf, len);
    close(fd);
    return 0;
}

//----------------------------------------------------------------
// gpio_set_dir
//----------------------------------------------------------------
int hw_gpio_set_dir(unsigned int gpio, unsigned int is_out)
{
  int fd, len;
  char buf[MAX_BUF];
  int ret;

  len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", gpio);
  fd = open(buf, O_RDWR | O_CLOEXEC);
  if (fd < 0) {
    perror("gpio/set-direction");
    return fd;
  }
  /* Write to "direction" will reset /gpioxxx/value so we should get the existing
   * value and by-pass setup if the direction is same. */
  memset(buf, 0x00, sizeof(buf));
  ret = read(fd, buf, MAX_BUF);
  if (ret > 0) {
    if (is_out) {
      if (strncmp(buf, "out", strlen("out"))) {
        ret = write(fd, "out", 4);
      }
    } else {
      if (strncmp(buf, "in", strlen("in"))) {
        ret = write(fd, "in", 3);
      }
    }
  }
  close(fd);

  return 0;
}
//----------------------------------------------------------------
// gpio_set_value
//----------------------------------------------------------------
int hw_gpio_set_value(unsigned int gpio, unsigned int value)
{
    int fd, len;
    char buf[MAX_BUF];
    int status;

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
    fd = open(buf, O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        perror("gpio/set-value");
        return fd;
    }
    if (value)
        status = write(fd, "1", 2);
    else
        status = write(fd, "0", 2);
    close(fd);

    return 0;
}
//----------------------------------------------------------------
// gpio_get_value
//----------------------------------------------------------------
int hw_gpio_get_value(unsigned int gpio, unsigned int *value)
{
    int fd, len;
    char buf[MAX_BUF];
    char ch;
    int status;
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(buf, O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        perror("gpio/get-value");
        return fd;
    }
    status = read(fd, &ch, 1);
    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }
    close(fd);
    return 0;
}

//----------------------------------------------------------------
// gpio_set_edge
//----------------------------------------------------------------
int hw_gpio_set_edge(unsigned int gpio, char *edge)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

    fd = open(buf, O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        perror("gpio/set-edge");
        return fd;
    }
    write(fd, edge, strlen(edge) + 1);
    close(fd);
    return 0;
}

//----------------------------------------------------------------
// Description:
//   Given a gpio number in /sys/class/gpio and return a opened file handle
// of this gpio.
//----------------------------------------------------------------
int hw_gpio_fd_open(unsigned int gpio)
{
    int fd, len;
    char buf[MAX_BUF];

    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
    /* fd = open(buf, O_RDONLY | O_NONBLOCK ); */
    fd = open(buf, O_RDWR | O_SYNC | O_CLOEXEC);
    // fd = open(buf, (O_RDWR | O_CLOEXEC | O_SYNC | O_DIRECT));
    if (fd < 0) {
        perror("gpio/fd_open");
    }

    return fd;
}

//----------------------------------------------------------------
// gpio_fd_close
//----------------------------------------------------------------
int hw_gpio_fd_close(int fd)
{
    return close(fd);
}
