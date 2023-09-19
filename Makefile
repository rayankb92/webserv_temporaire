 #===============================================================================#
#								SOURCES											  #
 #===============================================================================#

MAIN_DIR				= ./
CLASS_DIR				= ./classes/
STRING_HELP_DIR			= $(CLASS_DIR)LibHelp/
FILE_READER_DIR			= $(CLASS_DIR)FileReader/
PARSER_DIR				= $(CLASS_DIR)Parser/
STREAM_DIR				= $(CLASS_DIR)Stream/
LOCATION_DIR			= $(CLASS_DIR)Location/
HTTP_SERVER_DIR			= $(CLASS_DIR)HttpServer/
PAGE					= $(CLASS_DIR)Page/

 #=============================================================================#
INCLUDES				=	-Iincludes \
							-Iincludes/FileReader \
							-Iincludes/Parser \
							-Iincludes/LibHelp \
							-Iincludes/Stream \
							-Iincludes/Location \
							-Iincludes/HttpServer \
							-Iincludes/Page \

SRC_FILES = $(addprefix $(MAIN_DIR), main.cpp) \
			$(addprefix $(STRING_HELP_DIR), LibHelp.cpp) \
			$(addprefix $(FILE_READER_DIR), FileReader.cpp) \
			$(addprefix $(PARSER_DIR), Parser.cpp setters.cpp Methods.cpp Get.cpp Post.cpp Cgi.cpp Delete.cpp) \
			$(addprefix $(STREAM_DIR), Server.cpp) \
			$(addprefix $(STREAM_DIR), Stream.cpp) \
			$(addprefix $(STREAM_DIR), Client.cpp) \
			$(addprefix $(LOCATION_DIR), Location.cpp) \
			$(addprefix $(HTTP_SERVER_DIR), HttpServer.cpp) \
			$(addprefix $(PAGE), Page.cpp) \

 #=============================================================================#
#									OBJETS										#
 #=============================================================================#

OBJS_DIR = objects
OBJS = $(SRC_FILES:%.cpp=$(OBJS_DIR)/%.o)
DEPS = $(SRC_FILES:%.cpp=$(OBJS_DIR)/%.d)

 #=============================================================================#
#									COMPILATION									#
 #=============================================================================#

CC = c++
CFLAGS = -g3 -Wall -Wextra -Werror $(INCLUDES)
CDFLAGS = -MMD -MP

 #=============================================================================#
#									MAKEFILE									#
 #=============================================================================#

NAME = webserv

all : $(NAME)

$(NAME) : $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(CLFLAGS) -o $(NAME)

$(OBJS_DIR) :
	mkdir $(OBJS_DIR)
	mkdir -p $(OBJS_DIR)/$(STRING_HELP_DIR)
	mkdir -p $(OBJS_DIR)/$(FILE_READER_DIR)
	mkdir -p $(OBJS_DIR)/$(PARSER_DIR)
	mkdir -p $(OBJS_DIR)/$(STREAM_DIR)
	mkdir -p $(OBJS_DIR)/$(LOCATION_DIR)
	mkdir -p $(OBJS_DIR)/$(HTTP_SERVER_DIR)
	mkdir -p $(OBJS_DIR)/$(PAGE)

$(OBJS) : $(OBJS_DIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $(CDFLAGS) -c $< -o $@

clean :
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re : fclean all

-include $(DEPS)

.PHONY: all clean fclean re

#.SILENT: