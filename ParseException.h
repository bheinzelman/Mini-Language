/*
 * Exception class for Parser
 * Exceptions to be thrown when wrong 
 */
 
class ParseException : public std::exception {
  public:
		ParseException(const char *msg) : err_msg(msg) {};
		
		~ParseException() throw() {};
		
		const char *what() const throw() { return this->err_msg.c_str(); };
	private:
		std::string err_msg;
};