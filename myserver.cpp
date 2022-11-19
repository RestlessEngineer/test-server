#include <iostream>
#include <App.h>
#include <nlohmann/json.hpp>
#include <string_view>
#include <string>

using json = nlohmann::json;

namespace header
{
    const char* COMMAND = "command";
    const char* TEXT = "text";
    const char* USER_FROM = "user_from";
    const char* USER_TO = "user_to";
}

namespace command
{
    const char* PRIVATE_MSG = "private_msg";
    const char* PUBLIC_MSG = "public_msg";
}

// [USER 17] =server> {"command":"public_msg", "text":"a"}
// [SERVER] =everyone> {"command":"public_msg", "text":"a", "user_from": 17}
void process_public_msg(json parsed_data, auto *ws)
{
    auto *udata = ws->getUserData();
    json payload = {
        {header::COMMAND, command::PUBLIC_MSG},
        {header::TEXT, parsed_data[header::TEXT]},
        {header::USER_FROM, udata->id}};
    ws->publish("public", payload.dump());
}

// [USER 17] =server> {"command":"private_msg", "text":"hi", "user_to": 23}
// [SERVER] =user23> {"command":"private_msg", "text":"hi", "user_from": 17}
void process_private_msg(json parsed_data, auto *ws)
{
    auto *udata = ws->getUserData();

    json payload = {
        {header::COMMAND, command::PRIVATE_MSG},
        {header::TEXT, parsed_data[header::TEXT]},
        {header::USER_FROM, udata->id}};
    
    std::cout << "[server]: send message to user" << parsed_data[header::USER_TO].dump() << std::endl;
    ws->publish("user" + parsed_data[header::USER_TO].dump() , payload.dump());
}

int main()
{
    int latest_user_id = 10;

    /* ws->getUserData returns one of these */
    struct UserData
    {
        int id;
        std::string name;
    };

    // .ws = configuration of WebSocket server
    uWS::App().ws<UserData>("/*", {
            .idleTimeout = 16,
            /* Handlers */

            // Somebody connects to server
            .open = [&latest_user_id](auto *ws)
            {
            // Give user: id, name
            auto* data = ws->getUserData();
            data->id = latest_user_id++;
            data->name = "unnamed";
 
            std::cout << "[server]: New user connected: " << data->id << std::endl;
            ws->send("Welcome to test chat server, v0.1.0", uWS::OpCode::TEXT);
 
            ws->subscribe("public");
            ws->subscribe("user" + std::to_string(data->id)); // Private channel!
            },

            // Somebody sended data to server
            .message = [](auto *ws, std::string_view message, uWS::OpCode opCode)
            {
                json parsed_data = json::parse(message);

                if (parsed_data[header::COMMAND] == command::PUBLIC_MSG)
                {
                    process_public_msg(parsed_data, ws);
                }

                if (parsed_data[header::COMMAND] == command::PRIVATE_MSG)
                {
                    process_private_msg(parsed_data, ws);
                } },

            // Somebody disconnected from server
            .close = [](auto * /*ws*/, int /*code*/, std::string_view /*message*/)
            {
                /* You may access ws->getUserData() here */ }
            })
            .listen(9001, [](auto *listen_socket)
                    {
            if (listen_socket) {
                std::cout << "[server]: Listening on port " << 9001 << std::endl;
            } })
            .run();

}