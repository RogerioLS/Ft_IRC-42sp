# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/07 19:21:01 by codespace         #+#    #+#              #
#    Updated: 2025/06/26 11:14:18 by pmelo-ca         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = ircserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -g

SRC_DIR     = sources
OBJ_DIR     = build

SRCS        = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS        = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

INC_DIRS    = includes
INCLUDES    = -I$(INC_DIRS)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "✅ [Compiled] $(NAME)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "🧹 [Cleaned] Object files"

fclean: clean
	@rm -f $(NAME)
	@echo "🧹 [Cleaned] Executable"

re: fclean all

.PHONY: all clean fclean re
