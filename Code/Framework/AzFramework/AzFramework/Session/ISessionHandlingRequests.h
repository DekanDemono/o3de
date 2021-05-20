/*
 * All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
 * its licensors.
 *
 * For complete copyright and license terms please see the LICENSE at the root of this
 * distribution (the "License"). All use of this software is governed by the License,
 * or, if provided, by the license below or the license accompanying this file. Do not
 * remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 */

#pragma once

#include <AzCore/std/string/string.h>

namespace AzFramework
{
    //! SessionConnectionConfig
    //! The properties for handling join session request.
    struct SessionConnectionConfig
    {
        // A unique identifier for registered player in session.
        AZStd::string m_playerSessionId;

        // The DNS identifier assigned to the instance that is running the session.
        AZStd::string m_dnsName;

        // The IP address of the session.
        AZStd::string m_ipAddress;

        // The port number for the session.
        uint16_t m_port;
    };

    //! SessionConnectionConfig
    //! The properties for handling player connect/disconnect
    struct PlayerConnectionConfig
    {
        // A unique identifier for player connection.
        uint32_t m_playerConnectionId;

        // A unique identifier for registered player in session.
        AZStd::string m_playerSessionId;
    };

    //! ISessionHandlingClientRequests
    //! The session handling events to invoke multiplayer component handle the work on client side
    class ISessionHandlingClientRequests
    {
    public:
        // Handle the player join session process
        // @param  sessionConnectionConfig The required properties to handle the player join session process
        // @return The result of player join session process
        virtual bool HandlePlayerJoinSession(const SessionConnectionConfig& sessionConnectionConfig) = 0;

        // Handle the player leave session process
        virtual void HandlePlayerLeaveSession() = 0;
    };

    //! ISessionHandlingServerRequests
    //! The session handling events to invoke server provider handle the work on server side
    class ISessionHandlingServerRequests
    {
    public:
        // Handle the destroy session process
        virtual void HandleDestroySession() = 0;

        // Validate the player join session process
        // @param  playerConnectionConfig The required properties to validate the player join session process
        // @return The result of player join session validation
        virtual bool ValidatePlayerJoinSession(const PlayerConnectionConfig& playerConnectionConfig) = 0;

        // Handle the player leave session process
        // @param  playerConnectionConfig The required properties to handle the player leave session process
        virtual void HandlePlayerLeaveSession(const PlayerConnectionConfig& playerConnectionConfig) = 0;
    };
} // namespace AzFramework