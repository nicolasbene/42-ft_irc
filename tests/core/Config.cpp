#include "Config.h"

Config::Config(void)
{
	this->stop_on_fail = true;
	this->timeout = 1000;
}

Config::Config(const Config& config)
{
	*this = config;
}

Config& Config::operator=(const Config& config)
{
	this->stop_on_fail = config.doStopOnFail();
	this->timeout = config.getTimeout();

	return *this;
}

Config::~Config(void) {}

bool	Config::doStopOnFail(void) const
{
	return this->stop_on_fail;
}

uint	Config::getTimeout(void) const
{
	return this->timeout;
}

void	Config::setStopOnFail(bool val)
{
	this->stop_on_fail = val;
}

void	Config::setTimeout(uint new_timeout)
{
	this->timeout = new_timeout;
}
