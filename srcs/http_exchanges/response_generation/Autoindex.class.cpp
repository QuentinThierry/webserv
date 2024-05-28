#include "Autoindex.class.hpp"
#include "HttpResponse.class.hpp"

Autoindex::Autoindex(void) : _location_root("")
{}

Autoindex::Autoindex(Autoindex const &model) : _location_root("")
{
	(void)model;
}

Autoindex & Autoindex::operator=(Autoindex const & model)
{
	(void)model;
	return (*this);
}

static s_document_data _get_document_data(std::string &uri_target, struct dirent * document)
{
	s_document_data	doc_data = {};
	struct stat		doc_stat;
	std::string		file_name;

	file_name = uri_target +  document->d_name;
	
	if (stat(file_name.c_str(), &doc_stat))
 		throw (ExceptionHttpStatusCode(HTTP_500));
	if (HAS_READ_RIGHT(doc_stat.st_mode))
	{
		doc_data.is_reachable = true;
		doc_data.name = document->d_name;
		doc_data.size = doc_stat.st_size;
		doc_data.type = ((doc_stat.st_mode & S_IFMT) == S_IFDIR) ? IS_DIRECTORY : IS_FILE;
		doc_data.last_modified = doc_stat.st_mtim.tv_sec;
	}
	else
		doc_data.is_reachable = false;
	return (doc_data);
}

static bool	_compare_file_data_name(s_document_data const &doc_data1, s_document_data const &doc_data2)
{
	if (doc_data1.name == "..")
		return (true);
	if (doc_data1.type != doc_data2.type)
		return (doc_data1.type == IS_DIRECTORY);
	return (doc_data1.name.compare(doc_data2.name) < 0);
}

Autoindex::Autoindex( std::string const & location_root, std::string const & target ) 
	throw (ExceptionHttpStatusCode)
	: _location_root(location_root + (location_root[location_root.size() - 1] != '/' ? "/":""))
{
	DIR *			directory;
	struct dirent *	document;
	s_document_data	doc_data;

	_target = target + (target[target.size() - 1] != '/' ? "/":"");
	_uri_target = _location_root + _target;

	directory = opendir(_uri_target.c_str());
	if (!directory)
		throw (ExceptionHttpStatusCode(HTTP_500));
	
	document = readdir(directory);
	while (document)
	{
		doc_data = _get_document_data(_uri_target, document);
		if (doc_data.is_reachable && doc_data.name != ".")
			_documents_data.push_back(doc_data);
		document = readdir(directory);
	}
	closedir(directory);
	std::sort(_documents_data.begin(), _documents_data.end(), _compare_file_data_name);
}

std::vector<s_document_data>	& Autoindex::GetDocumentsData()
{
	return (_documents_data);
}

static std::string _new_line(std::string str)
{
	return (str + "\r\n");
}

std::string Autoindex::_generate_html_header( void )
{
	std::string html_header;

	html_header = "\
		<head>\r\n\
		<meta charset=\"UTF-8\">\r\n\
		<title>Index</title>\r\n\
			<style>\r\n\
			table tbody tr:hover {\r\n\
				box-shadow: inset 1px 1px 5px #d4d4d4;\r\n\
			}\r\n\
			</style>\r\n\
		</head>\r\n";
	return (html_header);
}

static std::string _generate_link(std::string link_text, std::string target_dest)
{
	std::string link;

	link = "<a href=\"";
	link += target_dest;
	link += "\"> ";
	link += link_text;
	link += "</a>";
	return (link);
}

static std::string _generate_table_cell(e_cell_type cell_type, std::string style_parameters, std::string content)
{
	std::string type_tag;

	type_tag = cell_type == CELL_NORMAL ? "td ":"th ";
	return (std::string ("<") + type_tag + " style=\"" + style_parameters + "\">" + content + "</" + type_tag +">");
}

static std::string _style_padding(const char *padding)
{
	return (std::string("padding-left: ") + padding + "; ");
}

static std::string _style_width(const char *width)
{
	return (std::string("max-width: ") + width + "; ");
}

static std::string _generate_table_header(void)
{
	std::string header_line;

	header_line = _new_line(std::string("<thead>"));
	header_line += _new_line(std::string("<tr style=\"text-align: left; text-decoration: underline; font-weight: bold\">"));
	header_line += _new_line(std::string("<td></td>"));
	header_line += _new_line(_generate_table_cell(CELL_HEADER, _style_width(MAX_WIDTH_NAME)
		+ _style_padding(PADDING_LEFT_NAME), "Name"));
	header_line += _new_line(_generate_table_cell(CELL_HEADER, _style_width(MAX_WIDTH_SIZE)
		+ _style_padding(PADDING_LEFT_SIZE), "Size (o)"));
	header_line += _new_line(_generate_table_cell(CELL_HEADER, _style_width(MAX_WIDTH_DATE)
		+ _style_padding(PADDING_LEFT_DATE) + "text-align: center;", "Last modification"));
	header_line += _new_line(std::string("</tr>"));
	header_line += _new_line(std::string("</thead>"));
	return (header_line);
}

static std::string _generate_table_line(std::string symbol, std::string name, std::string size, std::string date)
{
	std::string line;

	line = _new_line(std::string("<tr style=\"max-width:100%; text-align: left; font-size: 100%; margin: 5px 0 0 5px\"}>"));
	line += _new_line(_generate_table_cell(CELL_HEADER, _style_width("1em"), symbol));
	line += _new_line(_generate_table_cell(CELL_NORMAL, _style_width(MAX_WIDTH_NAME) 
		+ _style_padding(PADDING_LEFT_NAME) + "; font-weight: bold; overflow-wrap: break-word;", name));
	line += _new_line(_generate_table_cell(CELL_NORMAL, _style_width(MAX_WIDTH_SIZE) + _style_padding(PADDING_LEFT_SIZE), size));
	line += _new_line(_generate_table_cell(CELL_NORMAL, _style_width(MAX_WIDTH_DATE) + _style_padding(PADDING_LEFT_DATE), date));
	line += _new_line(std::string("</tr>"));
	return (line);
}

static std::string _get_last_modified(time_t &date)
{
	std::tm * ptm = std::localtime(&date);
	char date_str[32];
	// Format: 15.06.2009 20:20:00
	std::strftime(date_str, 32, "%d.%m.%Y  %H:%M:%S", ptm);
	return (date_str);
}

static	std::string _get_target_parent(std::string const &target)
{
std::string	target_parent;
	size_t		idx;

	target_parent = target;
	target_parent.erase(target_parent.end() -1);
	idx = target_parent.find_last_of("/");
	target_parent = target.substr(0, idx + 1);
	if (target_parent.empty())
		target_parent = "/";
	return 	(target_parent);
}

static std::string _generate_add_parent_link_line(std::vector<s_document_data> &_documents_data, std::string const &target)
{
	if (_documents_data.at(0).name == std::string("..") )
	{
		return (_generate_table_line("📁", _generate_link("..", _get_target_parent(target)), "", ""));
	}
	else
		return (_generate_table_line("", "<p>parent unreachable</p>", "", ""));
}

static std::string _generate_add_one_document_link_line(
	s_document_data &document_data,
	std::string const &target)
{
	std::string symbol;
	std::string link;
	std::string last_modified; 
	std::string size;

	if (document_data.type == IS_DIRECTORY)
	{
		symbol ="📁";
		size = "";
	}
	else
	{
		symbol = "🗎";
		size = ft_itoa(document_data.size);
	}
	link = _generate_link(document_data.name, target + document_data.name);
	last_modified = _get_last_modified(document_data.last_modified);

	return (_generate_table_line(symbol, link, size, last_modified));
}

static std::string _generate_add_all_documents_link_lines(
	std::vector<s_document_data> &documents_data,
	std::string const &target)
{
	std::string								all_documents_links;
	std::vector<s_document_data>::iterator	it;

	all_documents_links += _new_line("<tbody style=\"max-width:100vw\">");
	if (target != "/")
		all_documents_links += _generate_add_parent_link_line(documents_data, target);
	it = documents_data.begin();
	it += (it != documents_data.end() && it->name == "..");
	while (it != documents_data.end())
	{
		all_documents_links += _generate_add_one_document_link_line(*it, target);
		++it;
	}
	all_documents_links += _new_line("</tbody>");
	return (all_documents_links);
}

static std::string _generate_index_table(std::vector<s_document_data> &documents_data,
	std::string const &_target)
{
	std::string html_index_table;
	
	html_index_table += _new_line("<table style=\"margin:2 2vw 2 2vw; padding:0.5em 0.5em 0.5em 0.5em; border-radius: 0.5em; border-width:1px; border-color: rgb(40, 40, 40); box-shadow: 1px 1px 5px rgb(40, 40, 40); max-width: 90vw; overflow-wrap: break-word;\">");
	html_index_table += _generate_table_header();
	html_index_table += _generate_add_all_documents_link_lines(documents_data, _target);
	html_index_table += _new_line("</table>");
	return (html_index_table);
}

std::string Autoindex::_generate_html_body( void )
{
	std::string html_body;

	html_body = _new_line("<body style=\"font-family:sans-serif; padding-left:1%\">");

	html_body += _new_line(std::string("<h1> Index of ") + _target + "</h1>");
	html_body += _new_line("");

	html_body += _generate_index_table(_documents_data, _target);

	html_body += _new_line("</body>");
	return (html_body);
}

std::string	Autoindex::generateAutoIndexBody( void )
{
	std::string line;

	line = "<!DOCTYPE html>\r\n";
	line += "<html>\r\n";
	line += _generate_html_header();
	line += _generate_html_body();
	line += "</html>";
	return (line);
}
