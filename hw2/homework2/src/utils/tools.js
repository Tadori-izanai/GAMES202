function getRotationPrecomputeL(precompute_L, rotationMatrix){
	let result = Array.from(getMat3ValueFromRGB(precompute_L));

	let R = mat4Matrix2mathMatrix(rotationMatrix);
	R = math.transpose(R);
	let R3 = computeSquareMatrix_3by3(R);
	let R5 = computeSquareMatrix_5by5(R);

	for (let i = 0; i < 3; i += 1) {
		let slice3 = math.multiply(R3, Array.from(result[i].slice(1, 4)));
		for (let j = 0; j < 3; j += 1) {
			result[i][1 + j] = slice3[j];
		}
		let slice5 = math.multiply(R5, Array.from(result[i].slice(4)));
		for (let j = 0; j < 5; j += 1) {
			result[i][4 + j] = slice5[j];
		}
	}

	return result;
}

function computeSquareMatrix_3by3(rotationMatrix){ // 计算方阵SA(-1) 3*3 

	// 1、pick ni - {ni}
	let n1 = [1, 0, 0, 0]; let n2 = [0, 0, 1, 0]; let n3 = [0, 1, 0, 0];

	// 2、{P(ni)} - A  A_inverse
	let A = math.transpose(
		[
			SHEval(n1[0], n1[1], n1[2], 3).slice(1, 4),
			SHEval(n2[0], n2[1], n2[2], 3).slice(1, 4),
			SHEval(n3[0], n3[1], n3[2], 3).slice(1, 4)
		]
	);
	let A_inverse = math.inv(A);

	// 3、用 R 旋转 ni - {R(ni)}
	let rn1 = math.multiply(rotationMatrix, n1).toArray();
	let rn2 = math.multiply(rotationMatrix, n2).toArray();
	let rn3 = math.multiply(rotationMatrix, n3).toArray();
	
	// 4、R(ni) SH投影 - S
	let S = math.transpose(
		[
			SHEval(rn1[0], rn1[1], rn1[2], 3).slice(1, 4),
			SHEval(rn2[0], rn2[1], rn2[2], 3).slice(1, 4),
			SHEval(rn3[0], rn3[1], rn3[2], 3).slice(1, 4)
		]
	);

	// 5、S*A_inverse
	return math.multiply(S, A_inverse);
}

function computeSquareMatrix_5by5(rotationMatrix){ // 计算方阵SA(-1) 5*5
	
	// 1、pick ni - {ni}
	let k = 1 / math.sqrt(2);
	let n1 = [1, 0, 0, 0]; let n2 = [0, 0, 1, 0]; let n3 = [k, k, 0, 0]; 
	let n4 = [k, 0, k, 0]; let n5 = [0, k, k, 0];

	// 2、{P(ni)} - A  A_inverse
	let A = math.transpose(
		[	
			SHEval(n1[0], n1[1], n1[2], 3).slice(4),
			SHEval(n2[0], n2[1], n2[2], 3).slice(4),
			SHEval(n3[0], n3[1], n3[2], 3).slice(4),
			SHEval(n4[0], n4[1], n4[2], 3).slice(4),
			SHEval(n5[0], n5[1], n5[2], 3).slice(4)
		]
	);
	A_inverse = math.inv(A);

	// 3、用 R 旋转 ni - {R(ni)}
	let rn1 = math.multiply(rotationMatrix, n1).toArray();
	let rn2 = math.multiply(rotationMatrix, n2).toArray();
	let rn3 = math.multiply(rotationMatrix, n3).toArray();
	let rn4 = math.multiply(rotationMatrix, n4).toArray();
	let rn5 = math.multiply(rotationMatrix, n5).toArray();

	// 4、R(ni) SH投影 - S
	let S = math.transpose(
		[
			SHEval(rn1[0], rn1[1], rn1[2], 3).slice(4),
			SHEval(rn2[0], rn2[1], rn2[2], 3).slice(4),
			SHEval(rn3[0], rn3[1], rn3[2], 3).slice(4),
			SHEval(rn4[0], rn4[1], rn4[2], 3).slice(4),
			SHEval(rn5[0], rn5[1], rn5[2], 3).slice(4)
		]
	);

	// 5、S*A_inverse
	return math.multiply(S, A_inverse);
}

function mat4Matrix2mathMatrix(rotationMatrix){

	let mathMatrix = [];
	for(let i = 0; i < 4; i++){
		let r = [];
		for(let j = 0; j < 4; j++){
			r.push(rotationMatrix[i*4+j]);
		}
		mathMatrix.push(r);
	}
	return math.matrix(mathMatrix)

}

function getMat3ValueFromRGB(precomputeL){

    let colorMat3 = [];
    for(var i = 0; i<3; i++){
        colorMat3[i] = mat3.fromValues( precomputeL[0][i], precomputeL[1][i], precomputeL[2][i],
										precomputeL[3][i], precomputeL[4][i], precomputeL[5][i],
										precomputeL[6][i], precomputeL[7][i], precomputeL[8][i] ); 
	}
    return colorMat3;
}
