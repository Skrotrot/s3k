#include "backup.h"

#include "altc/string.h"
#include "kprintf.h"
#include "proc.h"
#include "shadow_stack.h"
#include <stdint.h>

extern unsigned char _payload[];

int procs_reset_count[S3K_PROC_CNT];
static bool backup_written[S3K_PROC_CNT];

static void *proc_mem_base(pid_t pid)
{
	return _payload + (uint64_t)pid * S3K_PROC_MEM_SIZE;
}

void process_backup_init(void)
{
	memset(_backup, 0, sizeof(backup_t) * S3K_PROC_CNT);
	memset(backup_written, 0, sizeof(backup_written));
}

bool process_backup_write(pid_t pid)
{
	proc_t *proc = proc_get(pid);
	backup_t *backup = &_backup[pid];

	memcpy(backup->regs, proc->regs, sizeof(backup->regs));
	memcpy(backup->mem, proc_mem_base(pid), S3K_PROC_MEM_SIZE);
	backup_written[pid] = true;

	return true;
}

bool process_backup_recover(pid_t pid)
{
	proc_t *proc = proc_get(pid);

	if (!backup_written[pid]) {
		kprintf(0, " === PID %d has no backup written, suspending it ===\n", pid);
		proc_suspend(proc);
		return false;
	}

	if (procs_reset_count[pid] >= S3K_PROC_MAX_RESETS) {
		kprintf(0, " === PID %d exceeded %d resets, suspending it ===\n", pid,
			S3K_PROC_MAX_RESETS);
		proc_suspend(proc);
		return false;
	}

	procs_reset_count[pid]++;
	kprintf(0, " === Resetting PID %d (reset %d/%d) ===\n", pid, procs_reset_count[pid], S3K_PROC_MAX_RESETS);

	backup_t *backup = &_backup[pid];
	memcpy(proc->regs, backup->regs, sizeof(backup->regs));
	memcpy(proc_mem_base(pid), backup->mem, S3K_PROC_MEM_SIZE);
	shadow_stack_reset(pid);

    proc_resume(proc);

	return true;
}
