/// This is about quickly getting one element of (A + changes)^(-1), given A^(-1).
/// Recalculating the whole matrix is slower but more straightforward,
/// see "Linear algebra" section for reference.
/// It seems that inner calculations for queries require much more precision
/// when compared to finding A^(-1), hence data types may be different
/// changes is in the (row, column) order! [[May be changed]]
template<class query_ans, class query_change, class orig>
query_ans perturbed_inverse(const vector<vector<orig>> &orig_inverse,
                            const vector<pair<pair<int, int>, query_change>> &changes,
                            const int row, const int col)
{
	/// Very fragile! Please, no changes with dx being equal to 0.
	/// Otherwise it is guaranteed to break down!
	vector<vector<query_ans>> mid(changes.size(), vector<query_ans>(changes.size()));
	for (int r = 0; r < int(changes.size()); r++)
		mid[r][r] = query_ans(1) / query_ans(changes[r].second);

	for (int r = 0; r < int(changes.size()); r++)
	{
		for (int c = 0; c < int(changes.size()); c++)
			mid[r][c] += orig_inverse[changes[r].first.second][changes[c].first.first];
	}

	vector<query_ans> left_mult(changes.size()), value_right(changes.size());
	for (int j = 0; j < int(changes.size()); j++)
	{
		left_mult[j] = orig_inverse[row][changes[j].first.first];
		value_right[j] = orig_inverse[changes[j].first.second][col];
	}

	/// Now, instead of finding u*M^(-1)*v, we solve Mx = v and find u*x.
	/// Runs faster when compared to explicitly inverting M.
	for (int j = 0; j < int(mid.size()); j++)
		mid[j].push_back(value_right[j]);
	auto right_mult = solve_system<query_ans>(mid);

	query_ans ans = orig_inverse[row][col];
	for (int j = 0; j < int(left_mult.size()); j++)
		ans -= query_ans(left_mult[j]) * query_ans(right_mult[j]);
	return ans;
}
