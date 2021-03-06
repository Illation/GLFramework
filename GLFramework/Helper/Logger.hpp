#pragma once

enum LogLevel
{
	Info = 0x1,
	Warning = 0x2,
	Error = 0x4,
	FixMe = 0x8
};

class Logger
{
private:
	class AbstractLogger
	{
	protected:
		ostream* m_os;
	public:
		AbstractLogger() {};
		virtual ~AbstractLogger() {};

		virtual void Log(const string& message)
		{
			(*m_os) << message;
			m_os->flush();
		}
	};

	class FileLogger : public AbstractLogger
	{
		string m_filename;
	public:
		explicit FileLogger(const string& fileName)
			: m_filename(fileName)
		{
			m_os = new ofstream(m_filename.c_str());
		}
		~FileLogger()
		{
			if (m_os)
			{
				ofstream* of = static_cast<ofstream*>(m_os);
				of->close();
				delete m_os;
			}
		}
	};

	class ConsoleLogger : public AbstractLogger
	{
	public:
		ConsoleLogger()
		{
			m_os = &std::cout;
		}
	};

	static ConsoleLogger* m_ConsoleLogger;
	static FileLogger* m_FileLogger;

	static unsigned char m_BreakBitField;
public:
	static void Initialize();
	static void Release();

	static void Log(const string& msg, LogLevel level = LogLevel::Info,
		bool timestamp = false, bool doBreak = false);
	static void APIENTRY LogFormat(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar* message, 
		const void* userParam);

	static void StartFileLogging(const string& filename);
	static void StopFileLogging();


private:
	//Disable default constructor and destructor
	Logger();
	~Logger();
	Logger(const Logger &obj);
	Logger& operator=(const Logger& obj);
};

