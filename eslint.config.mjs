import js from "@eslint/js";
import globals from "globals";
import { defineConfig } from "eslint/config";

export default defineConfig([
  {
    files: ["**/*.{js,mjs,cjs}"],
    plugins: { js },
    languageOptions: {
      ecmaVersion: "latest",
      sourceType: "module",
      globals: globals.node,
    },
  },
  {
    ignores: [
      "**/build/**", // compiled native addon binaries
      "**/dist/**", // JS output folder
      "**/node_modules/**",
      "**/venv/**",
      "**/*.cpp", // ignore native C++ files
      "**/*.h", // headers
    ],
  },
]);
