#pragma once

#include <unistd.h>
#include <sys/types.h>

class Config {
	public:
		Config(void);
		Config(const Config& config);
		Config& operator=(const Config& config);
		~Config(void);

		bool	doStopOnFail(void) const;
		uint	getTimeout(void) const;

		void	setStopOnFail(bool val);
		void	setTimeout(uint timeout);

	private:
		bool	stop_on_fail;
		uint	timeout;
};
