#pragma once

#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <iomanip>
#include <cstdlib>

#include "Config.h"
#include "color.h"

typedef void (*fun_ptr)(void);
typedef struct timeval t_timeval;

class Test {
	public:

		Test(void);
		Test(std::string name, fun_ptr fun);
		Test(const Test &test);
		Test &operator=(const Test &test);

		~Test(void);

		int			getPid(void) const;
		int			getStatus(void) const;
		double		getTime(void) const;
		std::string	getName(void) const;
		fun_ptr		getFun(void) const;
		bool		isFailed(void) const;

		bool		exec(const Config& config);
		void		print_trace(void);
		void		print_banner(void) const;

	private:
		int			pid;
		int			status;
		int			exit_code;
		bool		failed;
		double		time;
		std::string	name;
		fun_ptr		fun;
		int			stdout;
		int			stderr;
};

std::string	print_time(double time);
