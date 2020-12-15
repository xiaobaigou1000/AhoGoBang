import React from 'react';
import './App.css';
import GoBang from './GoBang';

function App() {

  return (
    <div className="page">
      <div className="game-header">
        <h1 className="title">
          Aho GoBang written in wasm
        </h1>
      </div>
      <GoBang />
    </div>

  );
}

export default App;
