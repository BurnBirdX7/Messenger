//
//
//

#include "MessageDao.h"

std::vector<MessageDao::Message> MessageDao::getAll()
{
    std::vector<Message> vec;
    pqxx::result res = mPool.query("SELECT * FROM messages");

    for (auto row : res) {
        Message msg;
        msg.setId          (row["id"].get<int>().value());
        msg.setSenderId    (row["sender_id"].get<int>().value());
        msg.setChatId      (row["chat_id"].get<int>().value());
        msg.setText        (row["text"].get<std::string>().value());
        msg.setTimeCreated (row["time_created"].get<time_t>().value());
        msg.setTimeUpdated (row["time_updated"].get<time_t>().value());
        msg.setStatus      (row["status_id"].get<Commons::Data::int2>().value());

        vec.push_back(msg);
    }

    return vec;
}

MessageDao::Message MessageDao::getById(int id)
{
    if(id < 1) {
        throw DbException("[MessageDao]::getById: id should be greater than 0");
    }

    pqxx::result res = mPool.query("SELECT * FROM messages WHERE id=" + std::to_string(id));
    auto entity = res.begin();

    Message msg;
    msg.setId          (entity["id"].get<int>().value());
    msg.setSenderId    (entity["sender_id"].get<int>().value());
    msg.setChatId      (entity["chat_id"].get<int>().value());
    msg.setText        (entity["text"].get<std::string>().value());
    msg.setTimeCreated (entity["time_created"].get<time_t>().value());
    msg.setTimeUpdated (entity["time_updated"].get<time_t>().value());
    msg.setStatus      (entity["status_id"].get<Commons::Data::int2>().value());

    return msg;
}

bool MessageDao::update(Message msg)
{
	mPool.query
	(
	    "  UPDATE messages"
        "  SET sender_id=" + std::to_string(msg.getSenderId()) +
        ", chat_id="       + std::to_string(msg.getChatId()) +
        ", text="          + msg.getText() + ""
        ", time_created="  + std::to_string(msg.getTimeCreated()) +
        ", time_updated="  + std::to_string(msg.getTimeUpdated()) +
        ", status_id="     + std::to_string(msg.getStatus()) +
        "  WHERE messages.id=" + std::to_string(msg.getId())
    );
	return true;
}

bool MessageDao::insert(Message msg)
{
    mPool.query
    (
        "INSERT INTO messages (id, sender_id, chat_id, text, time_created, time_updated, status_id) "
        "VALUES(DEFAULT," +
                std::to_string(msg.getSenderId()) + "," +
                std::to_string(msg.getChatId()) + "," +
                "\'" + msg.getText() + "\'," +
                std::to_string(msg.getTimeCreated()) + "," +
                std::to_string(msg.getTimeUpdated()) + "," +
                std::to_string(msg.getStatus()) + ")"
    );

    return true;
}

bool MessageDao::deleteById(int id)
{
    if(id < 1) {
        throw DbException("[MessageDao]::getById: id should be greater than 0");
    }

	mPool.query("DELETE FROM messages WHERE id=" + std::to_string(id));
	return true;
}
