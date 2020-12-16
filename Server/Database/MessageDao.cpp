//
//
//

#include "MessageDao.h"

std::optional<std::vector<MessageDao::Message>> MessageDao::getAll()
{
    std::optional<pqxx::result> res = mPool.query("SELECT * FROM messages");

    if(res.has_value()) {
        std::vector<Message> vec;
        for (auto row : res.value()) {
            Message msg;
            msg.setId(row["id"].get<int>().value());
            msg.setSenderId(row["sender_id"].get<int>().value());
            msg.setChatId(row["chat_id"].get<int>().value());
            msg.setText(row["text"].get<std::string>().value());
            msg.setTimeCreated(row["time_created"].get<time_t>().value());
            msg.setTimeUpdated(row["time_updated"].get<time_t>().value());
            msg.setStatus(row["status_id"].get<Commons::Data::int2>().value());

            vec.push_back(msg);
        }
        return vec;
    }
    return std::nullopt;
}

std::optional<MessageDao::Message> MessageDao::getById(int id)
{
    if(id < 1) {
        throw DbException("[MessageDao]::getById: id should be greater than 0");
    }

    std::optional<pqxx::result> res = mPool.query("SELECT * FROM messages WHERE id=" + std::to_string(id));
    if(res.has_value()) {
        auto entity = res.value().begin();

        Message msg;
        msg.setId(entity["id"].get<int>().value());
        msg.setSenderId(entity["sender_id"].get<int>().value());
        msg.setChatId(entity["chat_id"].get<int>().value());
        msg.setText(entity["text"].get<std::string>().value());
        msg.setTimeCreated(entity["time_created"].get<time_t>().value());
        msg.setTimeUpdated(entity["time_updated"].get<time_t>().value());
        msg.setStatus(entity["status_id"].get<Commons::Data::int2>().value());

        return msg;
    }
    return std::nullopt;
}

bool MessageDao::update(Message msg)
{
	if(mPool.query
	(
	    "  UPDATE messages"
        "  SET sender_id=" + std::to_string(msg.getSenderId()) +
        ", chat_id="       + std::to_string(msg.getChatId()) +
        ", text="          + msg.getText() + ""
        ", time_created="  + std::to_string(msg.getTimeCreated()) +
        ", time_updated="  + std::to_string(msg.getTimeUpdated()) +
        ", status_id="     + std::to_string(msg.getStatus()) +
        "  WHERE messages.id=" + std::to_string(msg.getId())
    ).has_value()) {
        return true;
    }
	return false;
}

bool MessageDao::insert(Message msg)
{
    if(mPool.query
    (
        "INSERT INTO messages (id, sender_id, chat_id, text, time_created, time_updated, status_id) "
        "VALUES(DEFAULT," +
                std::to_string(msg.getSenderId()) + "," +
                std::to_string(msg.getChatId()) + "," +
                "\'" + msg.getText() + "\'," +
                std::to_string(msg.getTimeCreated()) + "," +
                std::to_string(msg.getTimeUpdated()) + "," +
                std::to_string(msg.getStatus()) + ")"
    ).has_value()) {
        return true;
    }
    return false;
}

bool MessageDao::deleteById(int id)
{
    if(id < 1) {
        throw DbException("[MessageDao]::deleteById: id should be greater than 0");
    }

	if(mPool.query("DELETE FROM messages WHERE id=" + std::to_string(id)).has_value()) {
        return true;
    }
    return false;
}
