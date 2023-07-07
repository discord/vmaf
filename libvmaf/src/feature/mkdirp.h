
//
// mkdirp.h
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//

#ifndef MKDIRP
#define MKDIRP

#include <sys/types.h>
#include <sys/stat.h>

#ifdef DISCORD_WINDOWS_PORT
#define mode_t int
#endif

/*
 * Recursively `mkdir(path, mode)`
 */

int mkdirp(const char *, mode_t );

#endif
