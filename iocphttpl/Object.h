#pragma once


namespace resource{

	class Object
	{
	public:
		Object(void);
		~Object(void);
		int getId();
	protected:

	private:
		static int ctr;
		int Id;
	};

}