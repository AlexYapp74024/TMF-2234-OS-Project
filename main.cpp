#include <OpenXLSX.hpp>

#include <iostream>

int main()
{
	const std::string DOCUMENT_NAME = "Group.07 Mr. Rajan.xlsx";
	const std::string WORKSHEET_NAME = "Job.list.11";

	OpenXLSX::XLDocument doc;
	doc.open(DOCUMENT_NAME);

	auto workSheet = doc.workbook().worksheet(WORKSHEET_NAME);

	workSheet.cell("A1").value() = "Hello World!!!";
	OpenXLSX::XLCellRange range = workSheet.range(workSheet.cell("A1").cellReference(), workSheet.cell("A2").cellReference());
	//	range.begin()->value().set("AA");

	for (auto i = range.begin(); i != range.end(); i++)
	{
		i->value().set("AA");
	}

	doc.save();

	return 0;
}