package a03b.e1;

import java.util.ArrayList;
import java.util.List;
import java.util.function.BiFunction;
import java.util.function.Function;

/**
 * An interface to model several factories of Lenses, with many opportunities of reuse,
 * especially if one considers the idea of "composing lenses"
 *
 */
public class LensFactoryImpl implements LensFactory {

	/**
	 * @param <E>
	 * @param i
	 * @return a lens over the i-th element of a List<E>
	 */
	@Override
	public <E> Lens<List<E>, E> indexer(int i) {
		return new Lens<List<E>, E>(){
			
			@Override
			public E get(List<E> s) {
				return s.get(i);
			}

			@Override
			public List<E> set(E a, List<E> s) {
				ArrayList<E> list = new ArrayList<>(s);
				list.remove(i);
				list.add(i, a);
				return list;
			}
			
		};
	}

	/**
	 * @param <E>
	 * @param i
	 * @param i
	 * @return a lens of i-th row and j-th column of a List<List<E>>
	 */
	@Override
	public <E> Lens<List<List<E>>, E> doubleIndexer(int i, int j) {
		return new Lens<List<List<E>>, E>() {

			@Override
			public E get(List<List<E>> s) {
				ArrayList<List<E>> list = new ArrayList<List<E>>(s);
				return list.get(i).get(j);
			}

			@Override
			public List<List<E>> set(E a, List<List<E>> s) {
				ArrayList<List<E>> list = new ArrayList<List<E>>(s);
				ArrayList<E> sublist = new ArrayList<E>(list.get(i));
				sublist.set(j, a);
				list.set(i, sublist);
				return list;
			}
		};
	}

	/**
	 * @param <A>
	 * @param <B>
	 * @return a lens over the first component of a pair
	 */
	@Override
	public <A, B> Lens<Pair<A, B>, A> left() {
		return new Lens<Pair<A, B>, A>(){

			@Override
			public A get(Pair<A, B> s) {
				return s.get1();
			}

			@Override
			public Pair<A, B> set(A a, Pair<A, B> s) {
//				Pair<A, B> pair = new Pair<A, B>(a, s.get2());
//				return pair;
				return new Pair<A, B>(a, s.get2());
			}
			
		};
	}

	/**
	 * @param <A>
	 * @param <B>
	 * @return a lens over the second component of a pair
	 */
	@Override
	public <A, B> Lens<Pair<A, B>, B> right() {
		return new Lens<Pair<A, B>, B>() {

			@Override
			public B get(Pair<A, B> s) {
				return s.get2();
			}

			@Override
			public Pair<A, B> set(B b, Pair<A, B> s) {
//				Pair<A, B> pair = new Pair<A, B>(s.get1(), b);
//				return pair;
				return new Pair<A, B>(s.get1(), b);
			}
			
		};
	}

	/**
	 * @param <A>
	 * @param <B>
	 * @param <C>
	 * @param i
	 * @return a lens over the second component of the second component of the i-th element of a List<Pair<A,Pair<B,C>>> 
	 */
	@Override
	public <A, B, C> Lens<List<Pair<A, Pair<B, C>>>, C> rightRightAtPos(int i) {
		return new Lens<List<Pair<A, Pair<B, C>>>, C>(){
			
			@Override
			public C get(List<Pair<A, Pair<B, C>>> s) {
				return s.get(i).get2().get2();
			}

			@Override
			public List<Pair<A, Pair<B, C>>> set(C c, List<Pair<A, Pair<B, C>>> s) {
				ArrayList<Pair<A, Pair<B, C>>> list = new ArrayList<>(s);
//				Pair<A, Pair<B, C>> outerPair = s.get(i);
//				Pair<B,C> innerPair = outerPair.get2();
//				innerPair = new Pair<>(innerPair.get1(), c);
//				outerPair = new Pair<>(outerPair.get1(), innerPair);
				Pair<A, Pair<B, C>> pair = new Pair<>(s.get(i).get1(),
							new Pair<B, C>(s.get(i).get2().get1(), c));
				list.remove(i);
//				list.add(i, outerPair);
				list.add(i, pair);
				return list;
			}
			
		};
	}

}
