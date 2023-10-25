#include "Test.h"

Test::Test(void)
{
	this->name = "";
	this->fun = NULL;
	this->pid = 0;
	this->status = 0;
	this->time = 0;
	this->failed = false;
	this->stdout = 1;
	this->stdout = 2;
}

Test::Test(std::string name, fun_ptr fun)
{
	this->name = name;
	this->fun = fun;
	this->pid = 0;
	this->status = 0;
	this->time = 0;
	this->failed = false;
	this->stdout = 1;
	this->stdout = 2;
}

Test::Test(const Test& test)
{
	*this = test;
}

Test& Test::operator=(const Test& test)
{
	this->name = test.getName();
	this->fun = test.getFun();
	this->pid = test.getPid();
	this->status = test.getStatus();
	this->time = test.getTime();
	this->failed = test.isFailed();
	return (*this);
}

Test::~Test(void)
{
	if (this->stdout != STDOUT_FILENO)
		close(this->stdout);
	if (this->stderr != STDERR_FILENO)
		close(this->stderr);
}


int			Test::getPid(void) const
{
	return this->pid;
}

int			Test::getStatus(void) const
{
	return this->status;
}

double		Test::getTime(void) const
{
	return this->time;
}

std::string	Test::getName(void) const
{
	return this->name;
}

fun_ptr		Test::getFun(void) const
{
	return this->fun;
}

bool		Test::isFailed(void) const
{
	return this->failed;
}

static double	timediff(t_timeval end, t_timeval start)
{
	double	diff;

	diff = ((end.tv_sec - start.tv_sec) * 1000 + ((double)(end.tv_usec - start.tv_usec)) / 1000);
	return (diff / 1000);
}

void	Test::print_banner(void) const
{
	std::string	state;

	if (this->exit_code == 0)
		state = GREEN "PASS" RESET;
	else
		state = RED "FAIL" RESET;
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "        " << state << " [" << print_time(this->time) << "s]        " << BLUE + this->name + RESET;
	if (WIFSIGNALED(this->status)) {
		std::string	sig;
		if (this->exit_code == SIGSEGV)
			sig = "SIGSEGV";
		else if (this->exit_code == SIGABRT)
			sig = "SIGABRT";
		else if (this->exit_code == SIGBUS)
			sig = "SIGBUS";
		else if (this->exit_code == SIGPIPE)
			sig = "SIGPIPE";
		else if (this->exit_code == SIGUSR1)
			sig = "TIMEOUT";
		else
			sig = "";
		std::cout << " " RED << sig << RESET;
	}
	std::cout << std::endl;
}

static void	print_pipe(int fd)
{
	int		ret;
	char	buf[512 + 1];

	do {
		ret = read(fd, buf, 512);
		buf[ret] = 0;
		std::cout << buf;
	} while (ret != 0);
}

void	Test::print_trace(void)
{
	if (this->stdout == STDOUT_FILENO || this->stderr == STDERR_FILENO)
		return ;

	print_banner();

	if (this->failed) {
		std::cout << "\n" RED "--- STDOUT:" RESET "           \t\t" BLUE << this->name << RESET " " RED "---" RESET "\n";
		print_pipe(this->stdout);
		std::cout << "\n" RED "--- STDERR:" RESET "           \t\t" BLUE << this->name << RESET " " RED "---" RESET "\n";
		print_pipe(this->stderr);
		std::cout << "\n" RED "---" RESET "\n";
	}

	close(this->stderr);
	close(this->stdout);
	this->stdout = STDOUT_FILENO;
	this->stderr = STDERR_FILENO;
}

static int	settimeout(int pid, uint timeout)
{
	int	pid2;

	pid2 = fork();
	if (pid2 == 0) {
		usleep(timeout * 1000);
		kill(pid, SIGUSR1);
		exit(0);
	}
	return (pid2);
}

bool	Test::exec(const Config& config)
{
	int			timeout_pid;
	int			fd_out[2], fd_err[2];
	t_timeval	start, end;

	if (pipe(fd_out) == -1) return (false);
	if (pipe(fd_err) == -1) return (false);
	gettimeofday(&start, NULL);
	this->pid = fork();
	if (this->pid == -1) return (false);
	if (this->pid == 0) {
		close(fd_out[0]);
		close(fd_err[0]);
		dup2(fd_out[1], STDOUT_FILENO);
		dup2(fd_err[1], STDERR_FILENO);
		this->fun();
		exit(0);
	}
	this->stdout = fd_out[0];
	this->stderr = fd_err[0];
	close(fd_out[1]);
	close(fd_err[1]);
	timeout_pid = settimeout(this->pid, config.getTimeout());
	waitpid(this->pid, &this->status, 0);
	gettimeofday(&end, NULL);
	kill(timeout_pid, SIGKILL);
	this->time = timediff(end, start);
	if (WIFEXITED(this->status))
		this->exit_code = WEXITSTATUS(this->status);
	else if (WIFSIGNALED(this->status))
		this->exit_code = WTERMSIG(this->status);
	else
		this->exit_code = -1;
	if (this->exit_code != 0)
		this->failed = true;
	print_trace();
	return (true);
}

std::string	print_time(double time)
{
	if (time >= 100) {
		std::cout << " >=100";
		return "";
	}
	std::cout << std::fixed << std::setprecision(3);
	if (time < 10)
		std::cout << " ";
	std::cout << time;
	return "";
}
