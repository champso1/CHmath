import chmath;
import std;

int main()
{
	std::ofstream outfile("test.tbl");
	
	chmath::Table table(std::vector<std::string_view>{"x", "y", "z", "res"}, std::ref(outfile));
	table.set_width(8);
	std::vector<int> test_col(5, int{0});
	std::iota(test_col.begin(), test_col.end(), int{4});
	for (int i=0; i<3; ++i)
		table.add_column(test_col);
	std::vector<std::string> test_col2(5, "oijaw");
	table.add_column(test_col2);
	table.write();
	
	return 0;
}
