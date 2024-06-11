//database.h

#include<stdint.h>
#include<vector>
#include<string>
#include<memory>

namespace buried {
	class BuriedDbImpl; //����ʹ����pimpl���˼·
	class BuriedDb {
	public:
		struct data {
			int32_t ID;
			int32_t priority;
			uint64_t timestamp;
			std::vector<char> content; //���ﲻʹ��std::string��������sqlite_orm����ʹ��char
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