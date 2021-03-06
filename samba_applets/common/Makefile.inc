# ----------------------------------------------------------------------------
#         SAM Software Package License
# ----------------------------------------------------------------------------
# Copyright (c) 2015, Atmel Corporation
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# - Redistributions of source code must retain the above copyright notice,
# this list of conditions and the disclaimer below.
#
# Atmel's name may not be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
# DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

ifeq ($(CONFIG_SAMBA_APPLET),y)

CONFIG_TIMER_POLLING = y

CFLAGS_INC += -I$(TOP)/samba_applets/common

CFLAGS += -mno-unaligned-access

obj-y += samba_applets/common/applet_main.o
obj-y += samba_applets/common/applet_legacy.o

gnu-linker-script-$(CONFIG_SOC_SAMA5D2) = $(TOP)/samba_applets/common/sama5d2/sram.ld
gnu-debug-script-$(CONFIG_SOC_SAMA5D2) = $(TOP)/samba_applets/common/sama5d2/sram.gdb
iar-linker-script-$(CONFIG_SOC_SAMA5D2) = $(TOP)/samba_applets/common/sama5d2/sram.icf

gnu-linker-script-$(CONFIG_SOC_SAMA5D3) = $(TOP)/samba_applets/common/sama5d3/sram.ld
gnu-debug-script-$(CONFIG_SOC_SAMA5D3) = $(TOP)/samba_applets/common/sama5d3/sram.gdb
iar-linker-script-$(CONFIG_SOC_SAMA5D3) = $(TOP)/samba_applets/common/sama5d3/sram.icf

gnu-linker-script-$(CONFIG_SOC_SAMA5D4) = $(TOP)/samba_applets/common/sama5d4/sram.ld
gnu-debug-script-$(CONFIG_SOC_SAMA5D4) = $(TOP)/samba_applets/common/sama5d4/sram.gdb
iar-linker-script-$(CONFIG_SOC_SAMA5D4) = $(TOP)/samba_applets/common/sama5d4/sram.icf

gnu-cstartup-y = samba_applets/common/cstartup_gnu.S
iar-cstartup-y = samba_applets/common/cstartup_iar.s

iar-debug-script-y =
iar-flashloader-script-y =

endif
