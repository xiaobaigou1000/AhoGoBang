#include <vector>
#include <array>
#include <cstdint>
#include <algorithm>
#include <tuple>

class GoBang
{
public:
    GoBang() = default;

    bool placePiece(int32_t row, int32_t col, int32_t chessmanFlag)
    {
        if (!checkCanPlace(row, col))
            return false;
        board[row][col] = chessmanFlag;
        return true;
    }

    bool checkWinner(int32_t row, int32_t col)
    {
        if (!checkValid(row, col))
            return false;
        if (board[row][col] == 0)
            return false;
        return checkWinnerWithoutValidation(row, col);
    }

    auto getBoard() { return board; }

    auto getBoardLine()
    {
        std::vector<int32_t> inlineBoardVec(board.size() * board[0].size());
        for (int32_t i = 0; i < board.size(); i++)
        {
            std::copy(board[i].begin(), board[i].end(), inlineBoardVec.begin() + i * board[i].size());
        }
        return inlineBoardVec;
    }

    std::vector<int32_t> predictNext(int32_t pieceFlag, int32_t rivalFlag)
    {
        int32_t resultX = -1, resultY = -1, currentPrediction = -1;
        for (int32_t row = 0; row < boardSize; row++)
        {
            for (int32_t col = 0; col < boardSize; col++)
            {
                if (!checkCanPlace(row, col))
                    continue;

                auto predictFunc = [this](int32_t row, int32_t col, int32_t flag) {
                    auto predictionArr = calculatePiecesInLine<true>(row, col, flag);
                    return *std::max_element(predictionArr.begin(), predictionArr.end());
                };

                auto predictWeight = predictFunc(row, col, pieceFlag);
                auto rivalPrediction = predictFunc(row, col, rivalFlag);
                int32_t prediction = std::max(predictWeight, rivalPrediction);

                if (prediction > currentPrediction)
                {
                    currentPrediction = prediction;
                    resultX = row;
                    resultY = col;
                }
            }
        }
        std::vector<int32_t> result(2);
        result[0] = resultX;
        result[1] = resultY;
        return result;
    }

private:
    constexpr static int32_t boardSize = 32;
    std::vector<std::vector<int32_t>> board{boardSize, std::vector<int32_t>(boardSize, 0)};

    bool checkValid(int32_t row, int32_t col)
    {
        bool invalid = row < 0 || col < 0 || row >= boardSize || col >= boardSize;
        return !invalid;
    }

    bool checkCanPlace(int32_t row, int32_t col)
    {
        if (!checkValid(row, col))
            return false;
        if (board[row][col] != 0)
            return false;
        return true;
    }

    template <bool consequent>
    std::array<int32_t, 4> calculatePiecesInLine(int32_t row, int32_t col, int32_t chessFlag)
    {
        constexpr std::array<std::tuple<int32_t, int32_t>, 8> boardMoveVec{std::tuple<int32_t, int32_t>{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {-1, 1}, {1, -1}};
        std::array<int32_t, 4> pieceInLineCount{0};

        int32_t directionIndex = 0;
        for (const auto [rowDirection, colDirection] : boardMoveVec)
        {
            for (int i = 1; i < 5; ++i)
            {
                const int currentRow = row + rowDirection * i;
                const int currentCol = col + colDirection * i;
                if (!checkValid(currentRow, currentCol))
                    break;

                if (board[currentRow][currentCol] != chessFlag)
                {
                    if constexpr (consequent)
                        break;
                    else
                        continue;
                }
                pieceInLineCount[directionIndex / 2] += 1;
            }
            ++directionIndex;
        }
        return pieceInLineCount;
    }

    bool checkWinnerWithoutValidation(int32_t row, int32_t col)
    {
        int32_t chessFlag = board[row][col];
        auto pieceInLineCount = calculatePiecesInLine<true>(row, col, chessFlag);

        bool isWin = false;
        std::for_each(pieceInLineCount.begin(), pieceInLineCount.end(), [&isWin](int32_t i) {if(i>=4) isWin=true; });
        return isWin;
    }

    std::vector<std::vector<int32_t>> deprecatedPredictNextCount(int32_t pieceFlag)
    {
        std::vector<std::vector<int32_t>> result{boardSize, std::vector<int32_t>(boardSize, 0)};

        for (int32_t row = 0; row < boardSize; row++)
        {
            for (int32_t col = 0; col < boardSize; col++)
            {
                auto predictionArr = calculatePiecesInLine<false>(row, col, pieceFlag);
                auto predictionForMaxPiecesInLine = *std::max_element(predictionArr.begin(), predictionArr.end());
                result[row][col] = predictionForMaxPiecesInLine;
            }
        }
        return result;
    }
};
