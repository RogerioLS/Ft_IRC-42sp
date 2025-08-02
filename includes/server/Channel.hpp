/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 19:21:23 by codespace         #+#    #+#             */
/*   Updated: 2025/07/29 10:32:32 by pmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../utils/IRC.hpp"

class Server;

class Channel {

  private:
    bool              _inviteOnly;
    bool              _restrictTopic;
    int               _userLimit;
    int               _channelId;
    std::string       _name;
    std::string       _topic;
    std::string       _key;
    std::set<int>     _operatorsById;
    std::set<int>     _clientsById;
    std::set<int>     _clientsInvitedById;
    Debug&            _debug;

  public:
    Channel(std::string name, int channelId, int clientId, Debug& debug);
    ~Channel();

    // Getters
    bool getInviteOnly() const;
    bool getRestrictTopic() const;
    int getUserLimit() const;
    int getChannelId() const;
    const std::string & getName() const;
    const std::string & getTopic() const;
    const std::string & getKey() const;
    const std::set<int> & getOperatorsById() const;
    const std::set<int> & getClientsById() const;
    const std::set<int> & getInvitedById() const;

    // Setters
    void setInviteOnly(bool inviteOnly);
    void setRestrictTopic(bool restrictTopic);
    void setUserLimit(int userLimit);
    void setChannelId(int channelId);
    void setName(const std::string& name);
    void setTopic(const std::string& topic);
    void setKey(const std::string& key);
    void setOperatorsById(int operatorId);
    void setClientsById(int clientId);
    bool isClientInChannel(int clientId) const;
    void setClientsInvitedById(int clientId);
    void removeClient(int cliendId);
    void removeOper(int operId);
    void addOper(int operId);

    void  broadcastToAll(Server &server, const std::string & message);
};

#endif
