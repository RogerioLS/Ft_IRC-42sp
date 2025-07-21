# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmelo-ca <pmelo-ca@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/07 19:21:01 by codespace         #+#    #+#              #
#    Updated: 2025/06/29 19:16:34 by codespace        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nome do executável principal
NAME        = ircserv

# Compilador e flags
CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -g

# Diretórios
SRC_DIR     = sources
OBJ_DIR     = build
INC_DIRS    = includes
INCLUDES    = -I$(INC_DIRS)

# Arquivos de código-fonte
SRCS        = $(shell find $(SRC_DIR) -name "*.cpp")
OBJS        = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Configuração para testes
TEST_FLAGS  = -lgtest -lgtest_main -pthread
TEST_DIR    = tests
TEST_SRCS   = $(shell find $(TEST_DIR) -name "*.cpp")
TEST_OBJS   = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/tests/%.o, $(TEST_SRCS))

# Alvo principal
all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "✅ [Compiled] $(NAME)"

# Compilar arquivos de código-fonte
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "🧹 [Cleaned] Object files"

# Limpar tudo
fclean: clean
	@rm -f $(NAME)
	@echo "🧹 [Cleaned] Executable"

# Recompilar tudo
re: fclean all

.PHONY: all clean fclean re
