/*
 * Copyright 2011 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can
 * be found in the LICENSE file.
 */
#include <errno.h>
#include <assert.h>
#include "src/platform/nacl_exit.h"
#include "src/manifest/manifest_setup.h"
#include "src/service_runtime/nacl_globals.h"
#include "src/service_runtime/nacl_signal.h"
#include "src/platform/nacl_time.h"
#include "src/service_runtime/accounting.h"

static const char *zvm_state = UNKNOWN_STATE;

/* log user memory map */
static void LogMemMap(struct NaClApp *nap, int verbosity)
{
  int i;

  ZLOG(verbosity, "user memory map:");
  fflush(stdout);

  for(i = 0; i < MemMapSize; ++i)
  {
    ZLOG(verbosity, "%s: address = 0x%06x, size = %d, protection = x",
        nap->mem_map[RODataIdx].name,
        (uint32_t) nap->mem_map[RODataIdx].page_num,
        (uint32_t) nap->mem_map[RODataIdx].npages,
        nap->mem_map[RODataIdx].prot);
  }
}

static void FinalDump(struct NaClApp *nap)
{
  ZLOGS(LOG_INSANE, "exiting -- printing NaClApp details");

  /* NULL can be used because syslog used for nacl log */
  NaClAppPrintDetails(nap, (struct Gio *) NULL, LOG_INSANE);
  LogMemMap(nap, LOG_INSANE);

  if(nap->handle_signals) NaClSignalHandlerFini();
  NaClTimeFini();
}

/*
 * d'b: show dump (if needed). release resources, close channels.
 * note: use global nap because can be invoked from signal handler
 */
static void Finalizer(void)
{
  /* todo(d'b): get rid of hard coded "ok" */
  if(!STREQ(zvm_state, OK_STATE)) FinalDump(gnap);

  AccountingDtor(gnap);
  SystemManifestDtor(gnap);
  ProxyReport(gnap);
  ZLogDtor();
}

/* set the text for "exit state" in report */
void SetExitState(const char *state)
{
  assert(state != NULL);
  zvm_state = state;
}

/* get the "exit state" message */
const char *GetExitState()
{
  assert(zvm_state != NULL);
  return zvm_state;
}

void NaClExit(int err_code)
{
  Finalizer();
  ZLOGIF(err_code != 0, "zerovm exited with error '%s'", strerror(err_code));
  _exit(err_code);
}
