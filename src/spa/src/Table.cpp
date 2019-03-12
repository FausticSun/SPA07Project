#include "Table.h"
#include <algorithm>

Table::Table(HeaderRow headers) : headerRow(headers) {
  auto headersCopy = headers;
  std::sort(headersCopy.begin(), headersCopy.end());
  if (std::adjacent_find(headersCopy.begin(), headersCopy.end()) !=
      headersCopy.end()) {
    throw std::logic_error("Headers contain duplicate names");
  }
}

Table::Table(int noOfCols) {
  if (noOfCols < 0) {
    throw std::logic_error("No of columns is less that 0");
  }
  for (int i = 0; i < noOfCols; ++i) {
    headerRow.push_back(std::to_string(i));
  }
}

void Table::setHeader(HeaderRow headers) {
  if (headers.size() != headerRow.size()) {
    throw std::logic_error("Headers size mismatch");
  }

  auto headersCopy = headers;
  std::sort(headersCopy.begin(), headersCopy.end());
  if (std::adjacent_find(headersCopy.begin(), headersCopy.end()) !=
      headersCopy.end()) {
    throw std::logic_error("Headers contain duplicate names");
  }

  headerRow = headers;
}

void Table::modifyHeader(std::string oldHeader, std::string newHeader) {
  for (auto it = headerRow.begin(); it != headerRow.end(); ++it) {
    if (it->compare(oldHeader) == 0) {
      it->assign(newHeader);
      break;
    }
  }
}

void Table::insertRow(DataRow row) {
  if (row.size() != headerRow.size()) {
    throw std::logic_error("Data row size and header row size mismatch");
  }
  data.insert(row);
}

std::set<Table::DataRow> Table::getData(HeaderRow cols) const { return data; }

std::set<Table::DataRow> Table::getData(HeaderRow cols) const {
  std::vector<int> indices;
  std::set<DataRow> requestedData;

  // Get the indices of the required columns
  for (auto colName : cols) {
    auto it = std::find(headerRow.begin(), headerRow.end(), colName);
    if (it == headerRow.end()) {
      throw std::logic_error("Column: " + colName + " not found");
    }
    indices.push_back(std::distance(headerRow.begin(), it));
  }

  // Gets the requested data
  for (auto row : data) {
    DataRow requestedRow;
    for (auto i : indices) {
      requestedRow.push_back(row[i]);
    }
    requestedData.insert(requestedRow);
  }

  return requestedData;
}

bool Table::empty() const { return data.empty(); }

void Table::mergeWith(Table other) {
  // Other table is empty
  if (other.empty()) {
    return;
  }
  // This table is empty
  if (empty()) {
    data.insert(other.data.begin(), other.data.end());
    return;
  }
  // Get indices of common columns
  std::vector<std::pair<int, int>> commonIndices;
  std::vector<int> otherDiffIndices;
  for (int i = 0; i < headerRow.size(); ++i) {
    for (int j = 0; j < other.headerRow.size(); ++j) {
      if (headerRow[i] == other.headerRow[j]) {
        commonIndices.emplace_back(i, j);
        break;
      }
      otherDiffIndices.push_back(j);
    }
  }
  // Natural Join if there are common indices
  if (!commonIndices.empty()) {
    // Add non-common headers
    for (int i : otherDiffIndices) {
      headerRow.push_back(other.headerRow[i]);
    }
    // Iterate through DataRow in this table
    for (auto thisIt = data.begin(); thisIt != data.end(); ++thisIt) {
      // Find rows in other table that has the same data in common cols
      bool foundCommon = false;
      for (auto otherIt = other.data.begin(); otherIt != other.data.end();
           ++otherIt) {
        bool isCommon = true;
        for (auto indices : commonIndices) {
          if (thisIt->at(indices.first) != otherIt->at(indices.second)) {
            isCommon = false;
            break;
          }
        }
        // Row with common cols found, join
        if (isCommon) {
          auto dataRow = (*thisIt);
          for (auto i : otherDiffIndices) {
            dataRow.push_back(otherIt->at(i));
          }
          thisIt = data.erase(thisIt);
          thisIt = data.insert(thisIt, dataRow);
          otherIt = data.erase(otherIt);
          foundCommon = true;
        }
      }
      // No common cols, delete the row
      if (!foundCommon) {
        thisIt = data.erase(thisIt);
      }
    }
  }
  // Cartesian Product otherwise
  else {
    // Merge headers
    headerRow.assign(other.headerRow.begin(), other.headerRow.end());
    // Perform cartesian product
    std::set<DataRow> newData;
    for (auto thisRow : data) {
      for (auto otherRow : other.data) {
        DataRow newRow(thisRow.begin(), thisRow.end());
        newRow.assign(otherRow.begin(), otherRow.end());
        newData.insert(newRow);
      }
    }
  }
}

void Table::concatenate(Table other) {
  if (headerRow != other.headerRow) {
    throw std::logic_error("Headers are not equal");
  }
  for (auto row : other.data) {
    data.insert(row);
  }
}

void Table::setDifference(Table other) {
  if (headerRow != other.headerRow) {
    throw std::logic_error("Headers are not equal");
  }
  auto thisIt = data.begin();
  auto otherIt = other.data.begin();
  while (thisIt != data.end() && otherIt != other.data.end()) {
    if ((*thisIt) < (*otherIt)) {
      ++thisIt;
    } else if ((*thisIt) == (*otherIt)) {
      thisIt = data.erase(thisIt);
      ++otherIt;
    } else {
      ++otherIt;
    }
  }
}

void Table::transitiveClosure() {}
