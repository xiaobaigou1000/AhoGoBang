import React from 'react';
import './App.css';
import GoBang from './GoBang';

function App() {

  return (
    <div className="page">
      <div className="game-header">
        <h1 className="title">
          一个基于WASM的智障五子棋AI
        </h1>
      </div>
      <GoBang />
    </div>

  );
}

export default App;
