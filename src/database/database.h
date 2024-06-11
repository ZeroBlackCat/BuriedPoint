//database.h

#include<stdint.h>
#include<vector>
#include<string>
#include<memory>

namespace buried {
	class BuriedDbImpl; //这里使用了pimpl设计思路
	class BuriedDb {
	public:
		struct data {
			int32_t ID;
			int32_t priority;
			uint64_t timestamp;
			std::vector<char> content; //这里不使用std::string，是由于sqlite_orm中是使用char
		};
	public:
		BuriedDb(std::string dp_path);
		~BuriedDb();

		void InsertData(const data& data);
		void DeleteData(const data& data);
		void DeleteDatas(const std::vector<data>& datas);

		std::vector<BuriedDb::data> SelectDatas(int32_t limit);
	private:
		std::unique_ptr<BuriedDbImpl> impl_;
	};
}