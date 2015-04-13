/**************************************************************************/
/*  Copyright (C) MMLab@EE, CUHK, 2015
/*
/*  FILE NAME: common.h
/*  PRINCIPAL AUTHOR: Xin Zhu
/*  SUBSYSTEM NAME:
/*  MODULE NAME: common
/*  LANGUAGE: C
/*  TARGET ENVIRONMENT: Linux
/*  DATE OF FIRST RELEASE: 2015-04-12
/*  DESCRIPTION:
/**************************************************************************/


#ifndef COMMON_H
#define COMMON_H

/* standards system header file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


/* linux system header file */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <syslog.h>
#include <sys/stat.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <errno.h>

/* semaphore header file */
#include<semaphore.h>

/* pthread header file */
#include <pthread.h>



#endif


