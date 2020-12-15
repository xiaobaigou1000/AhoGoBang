import { useEffect, useState } from "react";
import "./Cell.css";
import "./Cells.css";
import "./GoBang.css";

export function Cell(props: any) {
    let row: number = props.row;
    let col: number = props.col;
    let flag: number = props.flag;
    let cellIdentity: number = props.cell;
    let setBoardStateFunc: (row: number, col: number, flag: number) => void = props.setStateFunc;

    return <div className={`cell cell-${cellIdentity}`} onClick={() => {
        setBoardStateFunc(row, col, flag);
    }}></div>;
}

export function Cells(props: any) {
    let flag: number = props.flag;
    let rowNumber: number = props.row;
    let rowArray: Array<number> = props.rowArr;
    let setBoardStateFunc: (row: number, col: number, flag: number) => void = props.setStateFunc;

    let cells = rowArray.map((value, index) => <Cell flag={flag} row={rowNumber} col={index} cell={value} setStateFunc={setBoardStateFunc}></Cell>);

    return <div className="cells-row">{cells}</div>;
}

export default function GoBang() {
    const [board, setBoard] = useState<Array<Array<number>>>([]);
    const [gobang, setGobang] = useState<any>();
    const [winner, setWinner] = useState("");
    let flag: number = 1;

    useEffect(() => {
        let tmp = Array.from(Array(32), () => new Int32Array(32));
        let b = tmp.map(value => Array.from(value));
        setBoard(b);

        require("./gobanglib").default().then((module: any) => setGobang(new module.GoBang()));
    }, []);

    let setBoardStateFunc = (row: number, col: number, currentFlag: number) => {
        if (!gobang) {
            return;
        }
        if (winner !== "") {
            return;
        }
        let myBoard = board.map(value => value.map(x => x));

        let ok: boolean = gobang.placePiece(row, col, currentFlag);
        if (!ok) {
            return;
        }
        myBoard[row][col] = currentFlag;
        setBoard(myBoard);

        let win: boolean = gobang.checkWinner(row, col);
        if (win) {
            setWinner("Player");
            return;
        }

        let predictVec = gobang.predictNext(currentFlag, currentFlag === 1 ? 2 : 1);
        let x: number = predictVec.get(0);
        let y: number = predictVec.get(1);
        predictVec.delete();

        ok = gobang.placePiece(x, y, currentFlag === 1 ? 2 : 1);
        if (!ok) {
            return;
        }
        myBoard[x][y] = currentFlag === 1 ? 2 : 1;
        setBoard(myBoard);

        win = gobang.checkWinner(x, y);
        if (win) {
            setWinner("Computer");
            return;
        }
    }

    useEffect(() => {
        console.log(gobang);
    }, [gobang]);

    let rows = board.map((rowVec: Array<number>, index: number) => <Cells flag={flag} row={index} rowArr={rowVec} setStateFunc={setBoardStateFunc}></Cells>);

    return <div className="game-container">
        {winner !== "" ? <div className="game-message">Winner: {winner}</div> : <></>}
        {rows}
    </div>;
}
