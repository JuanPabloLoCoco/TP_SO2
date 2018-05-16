#include <process_info.h>
#include <strings.h>
#include <process.h>
#include <scheduler.h>

int get_process_info_by_pid(process_info * pi, uint64_t pid)
{
	process * p = get_process_by_pid(pid);
	if (p == NULL)
  {
    return 0;
  }
	get_process_info(pi, p);
	return 1;
}

void get_process_info (process_info * pi, process * p)
{
	assign_quantum();

	pi->is_foreground = (get_foreground_process() == p);

	if (p == get_current_process())
		pi->state = RUNNING;
	else
		pi->state = is_blocked_process(p) ? BLOCKED : READY;
	pi->stack_addres = stack_page_process(p);
	pi->pid = pid_process(p);
	pi->ppid = ppid_process(p);

	get_name_process(pi->name, p);

	unassign_quantum();
}
